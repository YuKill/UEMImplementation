#! /bin/bash

arq=$1

output=`echo $arq | rev | cut -d '/' -f 1 | rev`
> $output
cat $arq | {
  while read benchName; do
    read status
    read time
    read cycles
    echo "$benchName $status $time $cycles" >> $output
  done
}
