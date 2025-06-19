#include <core.vaulta/core.vaulta.hpp>

namespace eosio {

void core::init(asset maximum_supply) {
   require_auth(get_self());
   config_table _config(get_self(), get_self().value);
   check(!_config.exists(), "This system contract is already initialized");

   auto sym = maximum_supply.symbol;
   check(maximum_supply.is_valid(), "invalid supply");
   check(maximum_supply.amount > 0, "max-supply must be positive");

   _config.set(config{.token_symbol = sym}, get_self());

   stats statstable(get_self(), sym.code().raw());
   statstable.emplace(get_self(), [&](auto& s) {
      s.supply     = maximum_supply;
      s.max_supply = maximum_supply;
      s.issuer     = get_self();
   });

   add_balance(get_self(), maximum_supply, get_self());
}

void core::transfer(const name& from, const name& to, const asset& quantity, const std::string& memo) {
   check(from != to, "cannot transfer to self");
   require_auth(from);
   check(is_account(to), "to account does not exist");

   auto        sym = quantity.symbol.code();
   stats       statstable(get_self(), sym.raw());
   const auto& st = statstable.get(sym.raw());

   check(quantity.is_valid(), "invalid quantity");
   check(quantity.amount > 0, "must transfer positive quantity");
   check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
   check(memo.size() <= 256, "memo has more than 256 bytes");

   auto payer = has_auth(to) ? to : from;

   sub_balance(from, quantity);
   add_balance(to, quantity, payer);

   require_recipient(from);
   require_recipient(to);

   // If `from` is sending XYZ tokens to this contract
   // they are swapping from XYZ to EOS
   if (to == get_self()) {
      check(quantity.symbol == get_token_symbol(), "Wrong token used");
      credit_eos_to(from, quantity);
   }
}

void core::add_balance(const name& owner, const asset& value, const name& ram_payer) {
   accounts to_acnts(get_self(), owner.value);
   auto     to = to_acnts.find(value.symbol.code().raw());
   if (to == to_acnts.end()) {
      to_acnts.emplace(ram_payer == owner ? owner : get_self(), [&](auto& a) {
         a.balance = value;
         a.released = ram_payer == owner;
      });
   } else {
      to_acnts.modify(to, same_payer, [&](auto& a) { a.balance += value; });
   }
}

void core::sub_balance(const name& owner, const asset& value) {
   accounts from_acnts(get_self(), owner.value);

   const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
   check(from.balance.amount >= value.amount, "overdrawn balance");

   if(!from.released){
      auto balance = from.balance;
      // This clears out the RAM consumed by the scope overhead.
      from_acnts.erase( from );
      from_acnts.emplace( owner, [&]( auto& a ){
         a.balance = balance - value;
         a.released = true;
      });
   } else {
      from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
   }
}

void core::on_transfer(const name& from, const name& to, const asset& quantity, const std::string& memo) {
   if (from == get_self() || to != get_self())
      return;
   check(quantity.amount > 0, "Swap amount must be greater than 0");

   // Ignore for system accounts, otherwise when unstaking or selling ram this will swap EOS for
   // XYZ and credit them to the sending account which will lock those tokens.
   if (from == "eosio.ram"_n)
      return;
   if (from == "eosio.stake"_n)
      return;

   check(quantity.symbol == EOS, "Invalid symbol");
   asset swap_amount = asset(quantity.amount, get_token_symbol());
   transfer_action(get_self(), {{get_self(), "active"_n}}).send(get_self(), from, swap_amount, std::string(""));
}

symbol core::get_token_symbol() {
   config_table _config(get_self(), get_self().value);
   check(_config.exists(), "Contract is not initialized");
   config cfg = _config.get();
   return cfg.token_symbol;
}

void core::credit_eos_to(const name& account, const asset& quantity) {
   check(quantity.amount > 0, "Credit amount must be greater than 0");

   asset swap_amount = asset(quantity.amount, EOS);
   transfer_action("eosio.token"_n, {{get_self(), "active"_n}}).send(get_self(), account, swap_amount, std::string(""));
}

void core::deposit(const name& owner, const asset& amount) {
   require_auth(owner);
   action(
      permission_level{ owner, "active"_n },
      "eosio"_n,
      "deposit"_n,
      std::make_tuple(owner, amount)
   ).send();
}

void core::withdraw(const name& owner, const asset& amount) {
   require_auth(owner);
   action(
      permission_level{ owner, "active"_n },
      "eosio"_n,
      "withdraw"_n,
      std::make_tuple(owner, amount)
   ).send();
}

void core::unstaketorex(const name& owner, const name& receiver, const asset& from_net, const asset& from_cpu) {
   require_auth(owner);
   action(
      permission_level{ owner, "active"_n },
      "eosio"_n,
      "unstaketorex"_n,
      std::make_tuple(owner, receiver, from_net, from_cpu)
   ).send();
}

void core::claimrewards(const name owner) {
   require_auth(owner);
   action(
      permission_level{ owner, "active"_n },
      "eosio"_n,
      "claimrewards"_n,
      std::make_tuple(owner)
   ).send();
}

} /// namespace eosio
