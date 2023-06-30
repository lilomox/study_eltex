get_libs: libsum.so libminus.so libmultiply.so libdevid.so

calc: calc.c
	gcc -Wall -o calc calc.c -ldl


libsum.so: sum.o
	gcc -shared -o libsum.so sum.o
sum.o: functions/sum.c
	gcc -fPIC -c -o sum.o functions/sum.c


libminus.so: minus.o
	gcc -shared -o libminus.so minus.o
minus.o: functions/minus.c
	gcc -fPIC -c -o minus.o functions/minus.c


libmultiply.so: multiply.o
	gcc -shared -o libmultiply.so multiply.o
multiply.o: functions/multiply.c
	gcc -fPIC -c -o multiply.o functions/multiply.c


libdevid.so: devid.o
	gcc -shared -o libdevid.so devid.o
devid.o: functions/devid.c
	gcc -fPIC -c -o devid.o functions/devid.c


clear:
	rm -f *.o
