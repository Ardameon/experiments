build:
	docker rmi my_test:test
	docker build --tag my_test:test .

run:
	docker run -v `pwd`:/app --name test_cont --rm my_test:test

stop:
	docker stop test_cont
	docker rm test_cont

.PHONY: run stop build
