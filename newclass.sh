#!/bin/bash

read -p "Enter class name: " classname
filename=`echo "$classname" | tr '[:upper:]' '[:lower:]'`

read -p "Prefix? " prefixname
classname="$prefixname""$classname"

OSNAME=`uname -s`

cp -f MyClass.cpp.template "$filename".cpp
cp -f MyClass.hpp.template "$filename".hpp

currenttime=`date '+%F %T'`
ESCAPED_REPLACE=$(printf '%s\n' "$currenttime" | sed -e 's/[\/&]/\\&/g')

if [[ "$OSNAME" == 'Darwin' || "$OSNAME" == "FreeBSD" ]]; then
    SUFFIX=".bak"
else
    SUFFIX=''
fi

sed -i$SUFFIX 's/\$filename\$/'$filename'/g' "$filename".cpp
sed -i$SUFFIX 's/\$filename\$/'$filename'/g' "$filename".hpp
sed -i$SUFFIX 's/\$classname\$/'$classname'/g' "$filename".cpp
sed -i$SUFFIX 's/\$classname\$/'$classname'/g' "$filename".hpp

sed -i$SUFFIX 's/\$time\$/'"$currenttime"'/g' "$filename".cpp
sed -i$SUFFIX 's/\$time\$/'"$currenttime"'/g' "$filename".hpp

if [[ "$SUFFIX" != '' ]]; then
    rm -f "$filename.cpp$SUFFIX" "$filename.hpp$SUFFIX"
fi
