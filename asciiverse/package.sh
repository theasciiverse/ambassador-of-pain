#!/bin/sh
# Packs Ambassador of Pain into its archive for av:
# dist/ambassador-of-pain-<target>.tar.gz (.zip for Windows targets), with
# the program, the eleven levels and the empty level to draw your own in
# levels/, our README, upstream's README as TRIVIA.md, the licence and the
# notes.
#   asciiverse/package.sh x86_64-unknown-linux-musl build/linux/aop
set -eu
cd "$(dirname "$0")/.."
target=$1
binary=$2
stage=stage/ambassador-of-pain
rm -rf "$stage" && mkdir -p "$stage/levels" dist
cp "$binary" "$stage/"
cp aop-level-*.txt empty-level.txt "$stage/levels/"
cp asciiverse/README.md "$stage/README.md"
cp README.md "$stage/TRIVIA.md"
cp LICENSE PATCHES.md "$stage/"
case $target in
  *windows*)
    rm -f "dist/ambassador-of-pain-$target.zip"
    (cd "$stage" && zip -q -r "../../dist/ambassador-of-pain-$target.zip" .) ;;
  *)
    tar --owner=0 --group=0 --numeric-owner -czf "dist/ambassador-of-pain-$target.tar.gz" -C "$stage" . 2>/dev/null \
      || tar -czf "dist/ambassador-of-pain-$target.tar.gz" -C "$stage" . ;;
esac
ls -l dist
