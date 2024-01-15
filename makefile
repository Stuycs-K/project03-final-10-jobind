compile: main.o nation.o player.o territory.o
	@gcc -o game main.o nation.o player.o territory.o
compile-client: client.o nation.o player.o territory.o
	@gcc -o client client.o nation.o player.o territory.o
server: compile
	@./game
client: compile-client
	@./client
client.o: client.c nation.h player.h territory.h
	@gcc -c client.c
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
	rm -rf client
	rm -rf *.region