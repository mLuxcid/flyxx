# disable default flags
override MAKEFLAGS += -rR

override TARGET := bin/flyxx

# default toolchain
override CC := clang
override LD := ld.lld

# user-overridable flags
override CFLAGS := -O3 -g -pipe
override LDFLAGS :=
override CPPFLAGS :=

# internal flags
override CFLAGS += \
	-Wall \
	-Wextra \
	-Werror \
	-std=gnu11 \
	-nostdinc \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

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
	-static \
	-z max-page-size=0x1000 \
	-gc-sections \
	-T linker.ld

# gather all files
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')

# we have a few extra files that are not part of the source code itself.
override CFILES += \
	printf/src/printf/printf.c

override OBJECTS := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))
 
override MISSING_DEPS := $(shell if ! test -d freestanding-headers || ! test -f src/cc-runtime.c || ! test -f src/limine.h; then echo 1; fi)
ifeq ($(MISSING_DEPS),1)
    $(error Please run the ./get-deps script first)
endif

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

-include $(HEADER_DEPS)

# recipe for C sources
obj/%.c.o: src/%.c
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

.PHONY: distclean
distclean:
	rm -rf bin-* obj-* freestanding-headers src/cc-runtime.c src/limine.h
