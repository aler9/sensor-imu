
all: build_cross

build_direct:
	@$(MAKE) -f Makefile.src

define DOCKERFILE
FROM balenalib/raspberry-pi-debian:stretch
RUN apt-get update && apt-get install -y --no-install-recommends \
    make \
    gcc \
    libc6-dev \
    libi2c-dev
WORKDIR /s
COPY *.h *.c Makefile Makefile.src ./
RUN make build_direct
endef
export DOCKERFILE

build_cross:
	docker run --rm --privileged multiarch/qemu-user-static:register --reset --credential yes >/dev/null
	echo "$$DOCKERFILE" | docker build . -f - -t temp

TARGET ?= pi@192.168.2.170
BIN = test-imu

deploy: build_cross
	ssh $(TARGET) "killall $(BIN) || exit 0"
	docker run --rm temp cat /s/$(BIN) \
	| ssh $(TARGET) "tee ./$(BIN) >/dev/null"
	ssh $(TARGET) "chmod +x ./$(BIN)"
	ssh $(TARGET) "./$(BIN)"
