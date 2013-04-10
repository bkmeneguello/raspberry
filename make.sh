#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

FILENAME=$(basename "$1")
EXTENSION="${FILENAME##*.}"
FILENAME="${FILENAME%.*}"

cc -o $DIR/bin/$FILENAME $1 -lwiringPi
