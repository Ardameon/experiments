build:
	docker build -t my_vim .

run:
	docker run -it --rm --volume $(shell pwd)/../..:/src_dir \
		--env UID=$(shell id -u) \
		--env GID=$(shell id -g) \
		my_vim

clean:
	docker rmi my_vim

.PHONY: run build clean
