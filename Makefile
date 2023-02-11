CC := gcc
CFLAGS := -lm -lraylib -std=gnu99 -Os -g

override CFILES := $(shell find ./src -type f -name '*.c')
override OFILES := $(shell find ./obj -type f -name '*.o')

all: ezdm

ezdm: 
	mkdir -p bin/ obj/
	$(foreach file, $(CFILES), $(CC) -c $(file) $(CFLAGS) -o obj/$(basename $(notdir $(file))).o;)
	$(CC) -o bin/ezdm $(OFILES) -lm -lraylib -lgcc

clean:
	rm -rf obj bin