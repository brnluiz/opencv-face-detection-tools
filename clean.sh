#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

echo $DIR

mkdir -p tmp
mkdir -p tmp/false-positives

rm tmp/*.jpg
rm tmp/false-positives/*.jpg
