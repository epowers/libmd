SRC = md2c.c md4c.c md5c.c md2hl.c md4hl.c md5hl.c\
      sha.c shahl.c rmd160.c rmd160hl.c
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -O3 -fPIC

all:	libmd.so.1.0 shadriver mddriver md2.3 md4.3 md5.3 libmd.a rmd160driver

clean:
	rm -f *.o core mddriver shadriver libmd.a libmd.so* md[245]hl.c\
	 md[245].3 shahl.c rmd160hl.c rmd160driver

install:	libmd.so.1.0 libmd.so.1 libmd.so
	install -c -g root -o root -m 644 md2.h md4.h md5.h sha.h rmd160.h \
			/usr/include
	install -c -g root -o root -m 644 md2.3 md4.3 md5.3 /usr/man/man3
	install -c -g root -o root -m 755 libmd.so.1.0 /usr/lib
	install -c -g root -o root -m 755 libmd.so.1 /usr/lib
	install -c -g root -o root -m 755 libmd.so /usr/lib
	install -c -g root -o root -m 755 libmd.a /usr/lib

libmd.so.1:
	ln -s libmd.so.1.0 libmd.so.1

libmd.so:
	ln -s libmd.so.1.0 libmd.so

md2hl.c: mdXhl.c
	sed -e 's/mdX/md2/g' -e 's/MDX/MD2/g' $? > $@

md4hl.c: mdXhl.c
	sed -e 's/mdX/md4/g' -e 's/MDX/MD4/g' $? > $@

md5hl.c: mdXhl.c
	sed -e 's/mdX/md5/g' -e 's/MDX/MD5/g' $? > $@

shahl.c: mdXhl.c
	sed -e 's/mdX/sha/g' -e 's/MDX/SHA/g' $? > $@

rmd160hl.c: mdXhl.c
	sed -e 's/mdX/rmd160/g' -e 's/MDX/RMD160/g' $? > $@

md2.3:	mdX.3
	sed -e 's/mdX/md2/g' -e 's/MDX/MD2/g' $? > $@
	cat md2.copyright >> $@

md4.3:	mdX.3
	sed -e 's/mdX/md4/g' -e 's/MDX/MD4/g' $? > $@
	cat md4.copyright >> $@

md5.3:	mdX.3
	sed -e 's/mdX/md5/g' -e 's/MDX/MD5/g' $? > $@
	cat md5.copyright >> $@

libmd.so.1.0:	$(OBJ)
	$(CC) -shared -o libmd.so.1.0 $(OBJ)

libmd.a:	$(OBJ)
	rm -f libmd.a
	ar rulv libmd.a $(OBJ)
	ranlib libmd.a

mddriver:	mddriver.o libmd.so
	$(CC) -fPIC -O3 -o mddriver mddriver.o -L. -lmd

shadriver:	shadriver.o libmd.so
	$(CC) -fPIC -O3 -o shadriver shadriver.o -L. -lmd

rmd160driver:	rmd160driver.o libmd.so
	$(CC) -fPIC -O3 -o rmd160driver rmd160driver.o -L. -lmd
