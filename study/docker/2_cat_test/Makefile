build:
	docker build --tag cat_test .

run_default:
	docker run --rm cat_test

run_readme:
	docker run --rm cat_test README

run_testfile:
	docker run --rm -v $(shell pwd):/data cat_test /data/testfile

.PHONY: build run_default run_readme run_testfile
