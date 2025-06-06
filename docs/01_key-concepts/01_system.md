---
title: System contracts, system accounts, privileged accounts
---

At the genesis of the Vaulta blockchain, there was only one account present, `eosio` account, which was and still is the main `system account`. During the Vaulta blockchain bootstrap process other `system account`s, were created by `eosio` account, which control specific actions of the `system contract`s. You can see them listed in the [About System Contract](../index.md#system-contracts-defined-in-system-contracts) section.

__Note__ the terms `system contract` and `system account`. `Privileged accounts` are accounts which can execute a transaction while skipping the standard authorization check. To ensure that this is not a security hole, the permission authority over these accounts is granted to `eosio.prods` system account.

As you just learned the relation between a `system account` and a `system contract`, it is also important to remember that not all system accounts contain a system contract, but each system account has important roles in the blockchain functionality, as follows:

|Account|Privileged|Has contract|Description|
|---|---|---|---|
|eosio|Yes|It contains the `eosio.system` contract|The main system account on the Vaulta blockchain.|
|eosio.msig|Yes|It contains the `eosio.msig` contract|Allows the signing of a multi-sig transaction proposal for later execution if all required parties sign the proposal before the expiration time.|
|eosio.wrap|Yes|It contains the `eosio.wrap` contract.|Simplifies block producer superuser actions by making them more readable and easier to audit.|
|eosio.token|No|It contains the `eosio.token` contract.|Defines the structures and actions allowing users to create, issue, and manage tokens on the Vaulta blockchain.|
|eosio.names|No|No|The account which is holding funds from namespace auctions.|
|eosio.bpay|No|No|The account that pays the block producers for producing blocks. It assigns 0.25% of the inflation based on the amount of blocks a block producer created in the last 24 hours.|
|eosio.prods|No|No|The account representing the union of all current active block producers permissions.|
|eosio.ram|No|No|The account that keeps track of the Vaulta balances based on users actions of buying or selling RAM.|
|eosio.ramfee|No|No|The account that keeps track of the fees collected from users RAM trading actions: 0.5% from the value of each trade goes into this account.|
|eosio.saving|No|No|The account which holds the 4% of network inflation.|
|eosio.stake|No|No|The account that keeps track of all Vaulta tokens which have been staked for voting.|
|eosio.vpay|No|No|The account that pays the block producers accordingly with the votes won. It assigns 0.75% of inflation based on the amount of votes a block producer won in the last 24 hours.|
|eosio.rex|No|No|The account that keeps track of fees and balances resulted from REX related actions execution.|
