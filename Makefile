all: Packet.cpp Record.cpp main.cpp helper.cpp base.h
	g++ Packet.cpp Record.cpp main.cpp helper.cpp -std=c++14 -g -o main
clean:
	rm main
