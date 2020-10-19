#!/bin/bash

echo [*] Scripts

cd Scripts
for i in $(ls *.sh); do  FILE=${i%.sh}.id; echo -n /tmp/${i%.sh}XXXXXX > $FILE  ; done
reswrap -S -a -k -N -o Scripts.hpp *.id *.sh
rm *.id

cd ..

echo [*] Resources

cd Resources
./MakeResources.sh

exit
