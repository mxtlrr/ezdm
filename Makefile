CC := gcc
CFLAGS := -lm -lraylib -std=gnu99 -Os -g -Iinclude

override CFILES := $(shell find ./src -type f -name '*.c')
override OFILES := $(shell find ./obj -type f -name '*.o')

all: ezdm

ezdm: 
	mkdir -p bin/ obj/
	@$(foreach file, $(CFILES), $(CC) -c $(file) $(CFLAGS) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	$(CC) -o bin/ezdm $(OFILES) -lm -lraylib -lgcc

clean:
	rm -rf obj bin