TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -Iinclude
LDFLAGS = -Map kernel.map -s -x -T kernel.ld
OBJS = main.o iv.o fbcon.o fb.o font.o kbc.o x86.o intr.o pic.o	\
	sched.o hpet.o acpi.o handler.o fs.o common.o

$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
%.o: %.s
	gcc $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	cp $(TARGET) ../fs/
	qemu-system-x86_64 -m 4G \
	-drive if=pflash,format=raw,readonly,file=$$HOME/ovmf/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$$HOME/ovmf/OVMF_VARS.fd \
	-hda fat:../fs

clean:
	rm -f *~ *.o *.map $(TARGET) include/*~

.PHONY: run clean
