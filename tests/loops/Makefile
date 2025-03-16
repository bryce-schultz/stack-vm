LIC=../../src/lic/main
VM=../../src/vm/main

all: test

compile: main.li for.li while.li
	../../build -j 32
	$(LIC) main.li
	$(LIC) for.li
	$(LIC) while.li

run: compile
	$(VM) main.lsm
	$(VM) for.lsm > for.output
	$(VM) while.lsm > while.output

test: run
	diff for.output for.correct
	diff while.output while.correct
	rm -f for.output while.output
	@echo "\033[0;32mAll tests passed!\033[0m"

clean:
	rm -f main.lsm for.lsm while.lsm