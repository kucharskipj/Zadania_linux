cmd_/home/user/zadanielinux/char_dev/my_device.ko := ld -r -m elf_x86_64 -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/user/zadanielinux/char_dev/my_device.ko /home/user/zadanielinux/char_dev/my_device.o /home/user/zadanielinux/char_dev/my_device.mod.o ;  true
