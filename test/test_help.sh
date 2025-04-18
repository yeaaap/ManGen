#!/bin/bash
set -e

echo "[TEST] Help message"

BIN_DIR=$(dirname "$0")/../build/debug
MANGEN="${BIN_DIR}/mangen"

"$MANGEN" -h > out.txt
grep -q "Usage:" out.txt

echo "Help message: OK"
rm out.txt
