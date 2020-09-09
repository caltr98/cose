gcc codediarray.c -c -o codediarray.o
ar rvs libcode.a codediarray.o 
gcc -Wall -pedantic  -c pipetok.c -o  pipetok.o
gcc pipetok.c -o myprog -L . -lcode -pthread
