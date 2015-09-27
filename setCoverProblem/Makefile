all: main.scala Table.scala Person.scala GeneticAlg.scala create
	@echo "Building..."
	@scalac -d build/ *.scala

create:
	@echo "#! /bin/bash" > run.sh
	@echo "scala -cp build/ scG.main \$$1 \$$2 \$$3 \$$4 \$$5 \$$6 \$$7 " >> run.sh
	@chmod +x run.sh

clean:
	@echo "Cleaning..."
	@rm -rf build
	@rm -f *.sh
	@mkdir build
