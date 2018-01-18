#!/bin/bash
# 
# Part 1: Parallel Overhead
printf "PART 1: Parallel Overhead\n"
./serial tests/16.txt
./simple_parallel 1 tests/16.txt
./serial tests/32.txt
./simple_parallel 1 tests/32.txt
./serial tests/64.txt
./simple_parallel 1 tests/64.txt
./serial tests/128.txt
./simple_parallel 1 tests/128.txt
./serial tests/256.txt
./simple_parallel 1 tests/256.txt
./serial tests/512.txt
./simple_parallel 1 tests/512.txt
./serial tests/1024.txt
./simple_parallel 1 tests/1024.txt

# Part 2: Parallel Speedup
printf "PART 2: Parallel Speedup\n"
./simple_parallel 2 tests/16.txt
./simple_parallel 4 tests/16.txt
./simple_parallel 8 tests/16.txt
./simple_parallel 16 tests/16.txt
./simple_parallel 32 tests/16.txt
./simple_parallel 64 tests/16.txt
./simple_parallel 2 tests/32.txt
./simple_parallel 4 tests/32.txt
./simple_parallel 8 tests/32.txt
./simple_parallel 16 tests/32.txt
./simple_parallel 32 tests/32.txt
./simple_parallel 64 tests/32.txt
./simple_parallel 2 tests/64.txt
./simple_parallel 4 tests/64.txt
./simple_parallel 8 tests/64.txt
./simple_parallel 16 tests/64.txt
./simple_parallel 32 tests/64.txt
./simple_parallel 64 tests/64.txt
./simple_parallel 2 tests/128.txt
./simple_parallel 4 tests/128.txt
./simple_parallel 8 tests/128.txt
./simple_parallel 16 tests/128.txt
./simple_parallel 32 tests/128.txt
./simple_parallel 64 tests/128.txt
./simple_parallel 2 tests/256.txt
./simple_parallel 4 tests/256.txt
./simple_parallel 8 tests/256.txt
./simple_parallel 16 tests/256.txt
./simple_parallel 32 tests/256.txt
./simple_parallel 64 tests/256.txt
./simple_parallel 2 tests/512.txt
./simple_parallel 4 tests/512.txt
./simple_parallel 8 tests/512.txt
./simple_parallel 16 tests/512.txt
./simple_parallel 32 tests/512.txt
./simple_parallel 64 tests/512.txt
./simple_parallel 2 tests/1024.txt
./simple_parallel 4 tests/1024.txt
./simple_parallel 8 tests/1024.txt
./simple_parallel 16 tests/1024.txt
./simple_parallel 32 tests/1024.txt
./simple_parallel 64 tests/1024.txt

