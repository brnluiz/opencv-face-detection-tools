#!/bin/bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

# Clean the old folder
rm -rf tmp/set2

# Create set2 images folder, if it doesn't exist
mkdir -p tmp/set2
mkdir -p tmp/set2/images

# Copy the false-positives + negative set
for i in `find tmp/set1/false-positives -name '*.jpg'` ; do cp $i tmp/set2/images ;  done
cp Data/train/neg/* tmp/set2/images

# Create the set2 image list
cd tmp/set2
find ./images -type f -maxdepth 1 > set2.lst
