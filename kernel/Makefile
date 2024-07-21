# disable default flags
override MAKEFLAGS += -rR

override TARGET := bin/flyxx

# default toolchain
override CC := cc
override LD := ld

# user-overridable flags
override CFLAGS := -O2 -g -pipe
override LDFLAGS :=
override CPPFLAGS :=

# internal flags
override CFLAGS += \
	-Wall \
	-Wextra \
	-Werror \
	-nostdinc \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fPIE \
	-m64 \
	-march=x86-64 \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone

override CPPFLAGS += \
	-Isrc \
	-Iprintf/src/ \
	-Ifreestanding-headers \
	-DPRINTF_SUPPORT_DECIMAL_SPECIFIERS=0 \
	-DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0 \
	-MMD \
	-MP

override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-pie \
	-z text \
	-z max-page-size=0x1000 \
	-T linker.ld

# gather all files
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')

# we have a few extra files that are not part of the source code itself.
override CFILES += \
	printf/src/printf/printf.c

override OBJECTS := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@


src/limine.h:
	curl -Lo $@ https://github.com/limine-bootloader/limine/raw/trunk/limine.h

-include $(HEADER_DEPS)

# recipe for C sources
obj/%.c.o: src/%.c src/limine.h
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# recipe for Assembly sources
obj/%.S.o: src/%.S
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# why do i have to do this maually
obj/printf/src/printf/printf.c.o: printf/src/printf/printf.c
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf bin/ obj/
