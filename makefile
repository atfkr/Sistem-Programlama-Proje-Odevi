make:kripto
kripto: main
	@echo sonra
	gcc -o kripto ./main.o ./libfdr/libfdr.a

main:
	@echo once
	gcc -c ./main.c
clean:
	rm -f *.o kripto
cleanall:clean
	rm -f .kilit encripted decripted *txt ornek_metin
run:clean make
	./kripto -e ./ornek_metin  ./encripted
	./kripto -d ./encripted ./decripted