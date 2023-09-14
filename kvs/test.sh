#!/bin/sh

find * -type d -name "test*" -exec rm -r {} +

make all

./kvs_main
