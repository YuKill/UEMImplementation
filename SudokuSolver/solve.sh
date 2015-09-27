#! /bin/bash

java -Xss512m -Xmx4g -cp build/classes/ main.SudokuSolver $@
