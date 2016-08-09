#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

echo $DIR

# Clean the dir
rm -rf tmp/set1

# Make the dir
mkdir -p tmp/set1
mkdir -p tmp/set1/false-positives
