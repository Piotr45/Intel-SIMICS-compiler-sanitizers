#!/bin/bash
export CROSS_PREFIX=riscv64-unknown-elf-
export PATH=$PATH:/opt/riscv/bin
make
rm ../snake64.elf
mv snake64.elf ../