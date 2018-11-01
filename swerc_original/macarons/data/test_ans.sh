#!/usr/bin/env bash

# if terminates silently, everything was fine
# first argument is the program to execute

for f in */*.in; do
  $1 < $f | diff - `dirname $f`/`basename $f .in`.ans
done
