#!/bin/sh

cat makefile | ./bin/ccd
echo -e "\n"
xxd makefile
