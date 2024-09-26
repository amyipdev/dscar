#!/usr/bin/env sh

cd "$(dirname "$0")"

for i in *.png; do
    filename=$(basename -- "$i")
    dbn="${filename%.*}"
    grit "$dbn".png -fts -o"$dbn".grit
done
