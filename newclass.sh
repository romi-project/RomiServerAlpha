#!/bin/bash

read -p "Enter class name: " classname
filename=`echo "$classname" | tr '[:upper:]' '[:lower:]'`

read -p "Prefix? " prefixname
classname="$prefixname""$classname"

cp -f MyClass.cpp.template "$filename".cpp
cp -f MyClass.hpp.template "$filename".hpp

currenttime=`date '+%F %T'`
ESCAPED_REPLACE=$(printf '%s\n' "$currenttime" | sed -e 's/[\/&]/\\&/g')

sed -i 's/\$filename\$/'$filename'/g' "$filename".cpp
sed -i 's/\$filename\$/'$filename'/g' "$filename".hpp
sed -i 's/\$classname\$/'$classname'/g' "$filename".cpp
sed -i 's/\$classname\$/'$classname'/g' "$filename".hpp

sed -i 's/\$time\$/'"$currenttime"'/g' "$filename".cpp
sed -i 's/\$time\$/'"$currenttime"'/g' "$filename".hpp

