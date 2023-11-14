#!/bin/bash

stat ex1

cp ex1 ex2

stat -c "Number of links: %h, I-node number: %i" ex1 ex2

# The i-node numbers for ex1 and ex2 will not be the same because a new copy (ex2) results a new i-node.
# Each file has a unique i-node number that is assigned when the file is created.
# Therefore, ex1 and ex2 will have different i-node numbers.

stat -c "%h - %n" /etc/* | grep '^3'

