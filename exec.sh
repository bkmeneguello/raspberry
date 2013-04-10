#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

FILENAME=$(basename "$1")
EXTENSION="${FILENAME##*.}"
FILENAME="${FILENAME%.*}"

$DIR/make.sh $1

sudo $DIR/bin/$FILENAME "$@"
