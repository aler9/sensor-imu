
.PHONY: $(shell ls)

help:
	@echo "usage: make [action]"
	@echo ""
	@echo "available actions:"
	@echo ""
	@echo "  format    format source files"
	@echo "  lint      run linter"
	@echo "  test      run tests"
	@echo ""

define DOCKERFILE_FORMAT
FROM ubuntu:20.04

RUN apt update && apt install -y --no-install-recommends \
	clang-format

ENTRYPOINT [ "sh", "-c", "clang-format \
	-i \
	--style=\"{BasedOnStyle: llvm, IndentWidth: 4}\"\
	*.c \
	*.h \
	orientation/*.c \
	orientation/*.h \
	test/*.c" ]

endef
export DOCKERFILE_FORMAT

format:
	echo "$$DOCKERFILE_FORMAT" | docker build - -t temp
	docker run --rm \
	-v $(PWD):/s \
	-w /s \
	temp

define DOCKERFILE_LINT
FROM ubuntu:20.04

RUN apt update && apt install -y --no-install-recommends \
	clang-format

ENTRYPOINT [ "sh", "-c", "clang-format \
	--dry-run \
	--Werror \
	--style=\"{BasedOnStyle: llvm, IndentWidth: 4}\"\
	*.c \
	*.h \
	orientation/*.c \
	orientation/*.h \
	test/*.c" ]

endef
export DOCKERFILE_LINT

lint:
	echo "$$DOCKERFILE_LINT" | docker build - -t temp
	docker run --rm \
	-v $(PWD):/s \
	-w /s \
	temp

test:
	cd test && make build-cross
