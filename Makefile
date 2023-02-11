CC := gcc
CFLAGS := -std=gnu99 -Os -g -Iinclude

LIBRARIES := -lm -lraylib -lpam

override CFILES := $(shell find ./src -type f -name '*.c')
override OFILES := $(shell find ./obj -type f -name '*.o')

all: ezdm

ezdm: 
	mkdir -p bin/ obj/
	@$(foreach file, $(CFILES), $(CC) -c $(file) $(CFLAGS) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	$(CC) -o bin/ezdm $(OFILES) $(LIBRARIES) -lgcc

clean:
	rm -rf obj bin