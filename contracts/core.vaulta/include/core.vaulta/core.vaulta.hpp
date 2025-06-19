#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

namespace eosio {
   class [[eosio::contract("core.vaulta")]] core : public contract {
      public:
         using contract::contract;
         using name   = eosio::name;
         using asset  = eosio::asset;
         using symbol = eosio::symbol;
         static constexpr symbol EOS = symbol("EOS", 4);

         struct [[eosio::table("accounts"), eosio::contract("core.vaulta")]] account {
            asset    balance;
            bool     released = false;
            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table("stat"), eosio::contract("core.vaulta")]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;

         struct [[eosio::table]] config {
            symbol token_symbol;
         };

         typedef eosio::singleton<"config"_n, config> config_table;


         [[eosio::action]] void init(asset maximum_supply);
         [[eosio::action]] void transfer(const name& from, const name& to, const asset& quantity, const std::string& memo);
         [[eosio::on_notify("eosio.token::transfer")]]
         void on_transfer(const name& from, const name& to, const asset& quantity, const std::string& memo);

         [[eosio::action]] void deposit(const name& owner, const asset& amount);
         [[eosio::action]] void withdraw(const name& owner, const asset& amount);
         [[eosio::action]] void unstaketorex(const name& owner, const name& receiver, const asset& from_net, const asset& from_cpu);
         [[eosio::action]] void claimrewards(const name owner);

         using init_action = eosio::action_wrapper<"init"_n, &core::init>;
         using transfer_action     = eosio::action_wrapper<"transfer"_n, &core::transfer>;
         using deposit_action = eosio::action_wrapper<"deposit"_n, &core::deposit>;
         using withdraw_action = eosio::action_wrapper<"withdraw"_n, &core::withdraw>;
         using unstaketorex_action = eosio::action_wrapper<"unstaketorex"_n, &core::unstaketorex>;
         using claimrewards_action = eosio::action_wrapper<"claimrewards"_n, &core::claimrewards>;

      private:
         void   add_balance(const name& owner, const asset& value, const name& ram_payer);
         void   sub_balance(const name& owner, const asset& value);
         symbol get_token_symbol();
         void   credit_eos_to(const name& account, const asset& quantity);
   };
} /// namespace eosio
