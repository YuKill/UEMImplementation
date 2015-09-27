#! /bin/bash

st=0
end=1
path=${1}
line=`cat $path | tr -d '!' | tr -d '-' | sed 's/\./0/g' | tr -d '\n'`
echo 3 > ${1}
for i in $(seq 0 8); do
  st=$(($i * 9))
  linePart=`echo ${line:$st:9} | fold -w1 | tr '\n' ' '`
  echo ${linePart} >> ${1}
done
