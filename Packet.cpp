//
// Created by 64419 on 11/26/2018.
//

#include "base.h"

int log2(int num) {
    switch (num) {
        case 2 : return 1;
        case 4 : return 2;
        case 8 : return 3;
        case 16 : return 4;
        case 32 : return 5;
        case 64 : return 6;
        default : return -1;
    }
}

void Packet::setContentInternal(const string &s) {
    vector<string> temp;
    splitByChar(s, temp, ',');
    timeStamp = stoull(temp[0], nullptr, 10);
    packetID = stoi(temp[1], nullptr, 10);
    sf = log2(stoi(temp[2], nullptr, 10)) + 6;
    rssi = stoi(temp[3], nullptr, 10);
    snr = stod(temp[4], nullptr);
}

Packet::Packet(const string &s) {
    setContentInternal(s);
}

Packet::Packet() {
    timeStamp = 0;
    packetID = 0;
    sf = 0;
    rssi = 0;
    snr = 0;
}

void Packet::setContent(const string &s) {
    setContentInternal(s);
}

int Packet::getSf() const { return sf; }

int Packet::getRssi() const { return rssi; }