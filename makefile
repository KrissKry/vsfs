CFLAG = gcc

all: main.o mkfs.o rmfs.o rm.o ls.o cptfs.o cpffs.o fsinfo.o
	$(CFLAG) main.o mkfs.o rmfs.o rm.o ls.o cptfs.o cpffs.o fsinfo.o -o vsfs

main.o: main.c
	$(CFLAG) -c main.c -o main.o

mkfs.o: mkfs.c
	$(CFLAG) -c mkfs.c -o mkfs.o

rmfs.o: rmfs.c
	$(CFLAG) -c rmfs.c -o rmfs.o

rm.o: rm.c
	$(CFLAG) -c rm.c -o rm.o

ls.o: ls.c
	$(CFLAG) -c ls.c -o ls.o

cptfs.o: cptfs.c
	$(CFLAG) -c cptfs.c -o cptfs.o

cpffs.o: cpffs.c
	$(CFLAG) -c cpffs.c -o cpffs.o

fsinfo.o: fsinfo.c
	$(CFLAG) -c fsinfo.c -o fsinfo.o