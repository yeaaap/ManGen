#!/bin/bash
set -e

echo "[TEST] Basic manifest generation"

BIN_DIR=$(dirname "$0")/../build/debug
MANGEN="${BIN_DIR}/mangen"

mkdir -p test_dir/dir_a
echo "hello" > test_dir/file1.txt
echo "world" > test_dir/dir_a/file2.txt

"$MANGEN" test_dir > out.txt

grep -q "file1.txt" out.txt
grep -q "dir_a/file2.txt" out.txt

echo "Basic manifest generation: OK"
rm -r test_dir out.txt
