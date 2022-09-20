#!/usr/bin/env bash

clang-tidy -checks="$(cat rules.txt | tr '\n' ',' | sed 's/,$//')" --dump-config > .clang-tidy
