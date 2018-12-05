all: main.cpp base.h helper.cpp Packet.cpp Record.cpp
	g++ Packet.cpp Record.cpp helper.cpp main.cpp -std=c++14 -g -o main
clean:
	rm main
