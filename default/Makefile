.DEFAULT_GOAL := all
export PACKROM := ../../../bin/packrom
.PHONY: all verify clean
all:
	python3 ../tools/build_editions.py
verify: all
	python3 ../tools/verify_release.py
clean:
	python3 ../tools/clean.py
