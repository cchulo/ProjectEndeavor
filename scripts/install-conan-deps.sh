#!/usr/bin/env bash

echo "$CONAN_SYSREQUIRES_MODE"

echo 'Restoring Conan packages'
conan install . -if bin/conan -of bin --profile default.jinja --profile compiler-linux-clang.jinja --profile host-arch.jinja --build=missing -s build_type=Release
