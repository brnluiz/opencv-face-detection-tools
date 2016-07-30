#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

echo $DIR

mkdir -p tmp/set1
mkdir -p tmp/set1/false-positives

rm tmp/set1/*.jpg
rm tmp/set1/false-positives/*.jpg
