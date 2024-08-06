override MAKEFLAGS += -rR

override QEMUFLAGS := \
	-M q35 \
	-m 2G \
	-boot d \
	-cdrom flyxx.iso

.PHONY: all
all: kernel

deps:
	build-support/build-limine.sh

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

.PHONY: run
run: iso
	qemu-system-x86_64 $(QEMUFLAGS)

.PHONY: run-kvm
run-kvm: iso
	qemu-system-x86_64 -cpu host -enable-kvm $(QEMUFLAGS)

iso: kernel deps
	build-support/generate-iso.sh

.PHONY: clean
clean:
	$(MAKE) -C kernel clean
	rm -rf *.iso

.PHONY: regenerate-deps
regenerate-deps:
	./build-support/build-limine.sh rebuild
