CURR_DIR := .

all: labs
	
check: run

.PHONY: labs
labs:
	cd src && $(MAKE) build

.PHONY: run
run: 
	./src/lab_work

clean:
	rm -rf lab_work