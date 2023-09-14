#!/bin/sh

rm -r testdb

make all

./kvs_main
