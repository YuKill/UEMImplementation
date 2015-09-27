#! /bin/bash

cd ../

bench=( 17 26 40 50 )
path="benchmark/9x9puzzle/"
> benchmarkResults/results9x9-$1
for i in ${bench[@]}; do
  for f in `ls $path | grep "${i}-"`; do
    ./solve.sh -i $path$f -a $1 -c $2 | tail -n 5 >> benchmarkResults/results9x9-$1
  done
done

cd benchmarkResults
