#!/bin/bash
set -e

echo "[TEST] Version output"

BIN_DIR=$(dirname "$0")/../build/debug
MANGEN="${BIN_DIR}/mangen"


"$MANGEN" -v > out.txt
grep -E -q "[0-9a-f]{7,}" out.txt
echo "Version output: OK"
rm out.txt
