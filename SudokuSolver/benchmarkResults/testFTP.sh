#! /bin/bash

cd ..

benchs=( 3 4 )
benchR="benchmarkResults"
> ${benchR}/resultsFTP-$1
for f in ${benchs[@]}; do
  echo $f
  ./solve.sh -i benchmark/FTPpuzzle/puzzle0${f}a.sudoku -a $1 -c $2 | tail -n 5 >> ${benchR}/resultsFTP-$1
  ./solve.sh -i benchmark/FTPpuzzle/puzzle0${f}b.sudoku -a $1 -c $2 | tail -n 5 >> ${benchR}/resultsFTP-$1
done

cd $benchR
