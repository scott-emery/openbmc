# Copyright 2015-present Facebook. All Rights Reserved.
all: fw-util

fw-util: fw-util.c
	$(CC) -pthread -lipmi -lipmb -lme -lgpio -lpal -lcpld -ljansson -std=c99 -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf *.o fw-util
