default: run
compile: main.o nation.o player.o territory.o
	@gcc -o game main.o nation.o player.o territory.o
run: compile
	@./game
main.o: main.c nation.h player.h territory.h
	@gcc -c main.c
nation.o: nation.c player.h territory.h
	@gcc -c nation.c
player.o: player.c nation.h territory.h
	@gcc -c player.c
territory.o: territory.c nation.h player.h
	@gcc -c territory.c
clean:
	rm -rf *.o
	rm -rf game