override MAKEFLAGS += -rR

override QEMUFLAGS := \
	-M q35 \
	-m 2G \
	-boot d \
	-cdrom flyxx.iso

.PHONY: all
all: kernel iso

limine:
	build-support/build-limine.sh

kernel-deps:
	./kernel/get-deps
	touch kernel-deps

.PHONY: kernel
kernel: kernel-deps
	$(MAKE) -C kernel

.PHONY: run
run: iso
	qemu-system-x86_64 $(QEMUFLAGS)

.PHONY: run-kvm
run-kvm: iso
	qemu-system-x86_64 -cpu host -enable-kvm $(QEMUFLAGS)

iso: kernel limine
	build-support/generate-iso.sh

.PHONY: regenerate-limine
regenerate-limine: limine
	./build-support/build-limine.sh rebuild

.PHONY: clean
clean:
	if test -f kernel-deps; then $(MAKE) -C kernel clean; fi
	rm -rf *.iso

.PHONY: distclean
distclean:
	if test -f kernel-deps; then $(MAKE) -C kernel clean; fi
	rm -rf *.iso limine kernel-deps
