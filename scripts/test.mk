define DOCKERFILE_TEST
FROM $(RPI_IMAGE)
RUN apt-get update && apt-get install -y --no-install-recommends \
    make \
    gcc \
    libc6-dev \
    libi2c-dev
WORKDIR /s
COPY . .
RUN make test-nodocker
endef
export DOCKERFILE_TEST

test:
	docker run --rm --privileged multiarch/qemu-user-static:register --reset --credential yes >/dev/null
	echo "$$DOCKERFILE_TEST" | docker build . -f - -t temp

test-nodocker:
	@$(MAKE) -C test -f Makefile.src
