CC := gcc
CFLAGS := -lm -lraylib -std=gnu99 -Os -g


all: ezdm

ezdm: src/ezdm.c
	mkdir -p obj/
	$(CC) $^ $(CFLAGS) -o obj/$@

clean: obj
	rm -rf $^