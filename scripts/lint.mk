define DOCKERFILE_LINT
FROM $(UBUNTU_IMAGE)

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
