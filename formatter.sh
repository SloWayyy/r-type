#!/bin/bash

find network -name '*.cpp' | xargs clang-format -i
echo "network done"
find client -name '*.cpp' | xargs clang-format -i
echo "client done"
find server -name '*.cpp' | xargs clang-format -i
echo "server done"