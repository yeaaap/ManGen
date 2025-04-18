#!/bin/bash
set -e

echo "[TEST] Exclusion of exact filename"

BIN_DIR=$(dirname "$0")/../build/debug
MANGEN="${BIN_DIR}/mangen"

mkdir -p test_dir
echo "keep" > test_dir/keep.txt
echo "exclude me" > test_dir/tmp.txt

"$MANGEN" test_dir -e tmp.txt > out.txt

grep -q "keep.txt" out.txt
! grep -q "tmp.txt" out.txt && echo "Exclusion: OK" || (echo "FAILED"; exit 1)

rm -r test_dir out.txt
