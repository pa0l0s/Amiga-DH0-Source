node: node.o
	g++ -o node obj/node.o  -LSYS:SDL_AGA/SDL/bin/ -lamiga -lpthread -noixemul
node.o: src/node.cpp
	g++ -o obj/node.o -c src/node.cpp -I SYS:source/yaml-cpp-master/include/
