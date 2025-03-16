LIC=../../src/lic/main
VM=../../src/vm/main

all: run

compile: main.li
	../../build -j 32
	$(LIC) main.li

run: compile
	$(VM) main.lsm

clean:
	rm -f main.lsm