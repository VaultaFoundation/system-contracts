---
title: How to buy RAM
link_text: How to buy RAM
---

## Goal

Setup an account that require multiple signatures for signing a transaction

## Before you begin

* You have an account

* Ensure the reference system contracts from `system-contracts` repository is deployed and used to manage system resources

* You have sufficient token allocated to your account

* Install the currently supported version of cleos

* Unlock your wallet

## Steps

Buys RAM in value of 10 EOS tokens for account `alice`:

```shell
cleos system buyram alice alice "10 EOS" -p alice@active
```