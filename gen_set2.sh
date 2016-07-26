#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

mkdir -p tmp/set2/images
mv tmp/false-positives/* tmp/set2/images
cp Data/train/neg/* tmp/set2/images

rm tmp/*.jpg
mv tmp/report.csv tmp/set2/report1.csv

cd tmp/set2
find ./images -type f -maxdepth 1 > set2.lst
