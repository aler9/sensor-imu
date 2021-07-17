
all: build-cross

build-direct:
	@$(MAKE) -f Makefile.src

define DOCKERFILE
FROM balenalib/raspberry-pi-debian:stretch
RUN apt-get update && apt-get install -y --no-install-recommends \
    make \
    gcc \
    libc6-dev \
    libi2c-dev
WORKDIR /s
COPY *.h *.c ./
COPY test ./test
WORKDIR ./test
RUN make build-direct
endef
export DOCKERFILE

build-cross:
	docker run --rm --privileged multiarch/qemu-user-static:register --reset --credential yes >/dev/null
	echo "$$DOCKERFILE" | docker build ../ -f - -t temp

TARGET ?= pi@192.168.2.170
BIN = test-imu

deploy: build-cross
	ssh $(TARGET) "killall $(BIN) || exit 0"
	docker run --rm temp cat /s/test/$(BIN) \
	| ssh $(TARGET) "tee ./$(BIN) >/dev/null"
	ssh $(TARGET) "chmod +x ./$(BIN)"
	ssh $(TARGET) "./$(BIN)"
