define DOCKERFILE_FORMAT
FROM $(UBUNTU_IMAGE)

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
	-v $(shell pwd):/s \
	-w /s \
	temp
