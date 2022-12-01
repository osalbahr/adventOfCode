#!/bin/bash

## Creates a folder for a problem, using $1
# "template" directory must be in the same directory
# example: ./create.sh day1

# Create the folder from template
cp -r template $1
cd $1

# Rename hello.cpp
cp hello.cpp $1.cpp
mv hello.cpp $1_2.cpp

# Edit the Makefile accordingly
str="s/hello/$1 $1_2/g"
sed -i -e "$str" Makefile

# I have no clue why sed created this
rm Makefile-e

# ignore executables
echo $1 >> .gitignore
echo $1_2 >> .gitignore

# Create three empty input and output files
touch {1,2,3}.{IN,OUT}
