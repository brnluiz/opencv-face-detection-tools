#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

# Clean old set 2 images
rm tmp/set2/images/*.jpg
rm tmp/set2/set2.lst
rm tmp/set2/report*.csv

# Create set2 images folder, if it doesn't exist
mkdir -p tmp/set2/images

# Copy the false-positives + negative set
mv tmp/set1/false-positives/*.jpg tmp/set2/images
cp Data/train/neg/* tmp/set2/images

# Create the set2 image list
cd tmp/set2
find ./images -type f -maxdepth 1 > set2.lst
