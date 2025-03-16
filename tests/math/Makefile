LIC=../../src/lic/lic
VM=../../src/vm/vm

all: run

compile: main.li
	$(LIC) main.li

run: compile
	$(VM) main.lsm

clean:
	rm -f main.lsm
	rm -f output

test: compile
	$(VM) main.lsm > output
	diff -qZ output expected