 ----------------------------------------
| Do an 02 - He dieu hanh
| Cu nhan Tai nang khoa 2016
| SV1: Vu Le The Anh (1612838)
| SV2: Nguyen Le Hong Hanh (1612849)
 ----------------------------------------

Yeu cau 1: system call
----------------------------------------

Luu y 1: tat ca deu chay duoi quyen root
Luu y 2: may phai co it nhat 35GB dung luong trong

Buoc 1: Tai kernel va giai nen o /usr/src
- Lan luot chay tung doan lenh duoi day:
	wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.16.36.tar.xz
	tar -xvf linux-3.16.36.tar.xz -C /usr/src/
	cd /usr/src/linux-3.16.36

Buoc 2: Them syscall vao Makefile cua kernel
- Chep thu muc pname va thu muc pid vao thu muc kernel vua giai nen
- O thu muc goc cua kernel (linux-3.16.36), chay lenh:
	cat -n Makefile | grep -i core-y 
- Tim dong core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ va ghi nho so dung truoc
- Chay lenh
	nano +(so lay duoc tu buoc tren) Makefile
- Them vao dang sau "crypto/" doan " pname/ pid/", luu y dau cach va dau /:
	 core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ pname/ pid/

Buoc 3: Them syscall vao syscall table:
- O thu muc goc cua kernel, chay lenh:
	nano arch/x86/syscalls/syscall_64.tbl
- Them vao dong cuoi cung cua cac dong danh so 3xx:
	350	common	pname	pnametoid
	351	common	pid	pidtoname

Buoc 4: (optional) Them vao syscall header:
- O thu muc goc cua kernel, chay lenh:
	nano include/linux/syscalls.h
- Them vao cuoi cung, ngay tren "#endif":
	asmlinkage int pnametoid(char* name);
	asmlinkage int pidtoname(int pid, char* buf, int len);

Buoc 5: Build
- O thu muc goc cua kernel, chay lenh:
	make menuconfig
- Sau khi chay xong, o bang hien ra, chon Save, OK, va Exit
- Tiep tuc chay lenh: (thay 4 bang so nhan)
	make -j 4
- Cho mot khoang thoi gian kha lau
- Cuoi cung, lan luot chay:
	make install -j 4
	make modules_install install
- Khoi dong lai va chon khoi dong bang kernel vua build:
	reboot

Buoc 6: Kiem tra
- Mo Terminal o duong dan thu muc nop va go:
	./test_pname
	./test_pid
- Lam theo huong dan

Yeu cau 2: rootkit
----------------------------------------

Buoc 1: Tim dia chi bang system call
- Go lenh:
	cat /boot/System.map-3.16.36 | grep sys_call_table
- Luu phan ma hex o dau sys_call_table

Buoc 2: Chinh sua ma nguon
- Vao tap tin hook.c, tim doan:
	system_call_table_addr = (void*) [___];
- Dien vao [___] doan ma hex tim duoc tu buoc 1

Buoc 3: Make
- Mo terminal o thu muc co file hook.c, go:
	make
- Luu y tap tin hook.ko duoc tao thanh

Buoc 4: Kiem tra
- Mo hai Terminal
- Terminal thu nhat dung de theo doi log dmesg, go:
	dmesg -C
	dmesg -wH
- Terminal thu hai de install module vua tao, go:
	insmod hook.ko
- Khi can ket thuc, go module:
	rmmod hook.ko
