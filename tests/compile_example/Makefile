LIC=../../build/src/compiler/compiler
VM=../../build/src/vm/vm

all: run

compile: main.li
	$(LIC) main.li

run: compile
	$(VM) main.li.lsm

clean:
	rm -f main.li.lsm