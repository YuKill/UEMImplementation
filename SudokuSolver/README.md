#_Sudoku Solver_

#####Usage: 
```sh
./solve.sh [arguments] [options]
```
|Arguments|Description|
|--------:|:------------|
|-i|specify the input file|
|-a|specify the algorithm to be used|

|Algorithms|Description|
|---------:|:----------|
|greedy|use greedy algorithm to solve| 
|genetic|use genetic algorithm to solve|
|backtrack|use backtrack to solve|

|Options|Description|
|------:|:----------|
|-v|shows the solution found(if found) or its closest answer|
|-c|specify the maximum cycles|

#####Examples: 
```sh
./solve.sh -i sudokuFile -a genetic -v 
```
Uses the genetic algorithm to try to solve the file 'sudokuFile'and at the end, it shows the answer it found
