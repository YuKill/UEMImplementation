#! /bin/bash

arq=$1

output=`echo $arq | rev | cut -d '/' -f 1 | rev`
> $output
cat $arq | {
  while read conf; do
    read benchName
    read status
    read time
    read cycles
    echo "$benchName $status $time $cycles" >> $output
  done
}
