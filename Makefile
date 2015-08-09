PHP_SRC_PATH=/home/vagrant/setup/php-5.4.29
PHP_EXT_PATH=/opt/software/php/lib/php/extensions/no-debug-non-zts-20100525/

INCLUDE=-I$(PHP_SRC_PATH) -I$(PHP_SRC_PATH)/main -I$(PHP_SRC_PATH)/TSRM -I$(PHP_SRC_PATH)/Zend -I$(PHP_SRC_PATH)/SAPI
CC=gcc
all: ljdebuger.so install restart

ljdebuger.so: ljdebuger.o
	$(CC) -shared -rdynamic -o ljdebuger.so ljdebuger.o
ljdebuger.o: ljdebuger.c *.h
	$(CC) -g -fpic  $(INCLUDE) -c ljdebuger.c 

clean:
	rm -fr *.so *.o

install: ljdebuger.so
	cp -fp ljdebuger.so $(PHP_EXT_PATH)

restart:
	pkill -9 php-fpm
	/opt/software/php/sbin/php-fpm
	php test.php
