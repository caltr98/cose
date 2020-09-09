CC		=  gcc
CFLAGS= -Wall -pedantic
LDFLAGS 	= -L.
AR = ar
ARFLAGS= rvs
SUFFIX= -g -pthread

.PHONY:all clean test
TARGET		= supermercato
myprog:
	bash compila.sh

all : $(TARGET)
	

clean:
	-rm -f  myprog
