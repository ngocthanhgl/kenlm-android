#!/usr/bin/env bash
set -euo pipefail

# Local test script for building kenlm_jni
# Requires: cmake, ninja, C++ compiler, boost-dev

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

if [ ! -d kenlm ]; then
    echo "Cloning KenLM..."
    git clone --depth 1 https://github.com/kpu/kenlm.git
fi

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --target kenlm_jni -j"$(nproc)"
echo "Done! .so at: $(find build -name '*.so' 2>/dev/null | head -1)"
