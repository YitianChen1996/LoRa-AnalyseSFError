//
// Created by 64419 on 11/26/2018.
//

#ifndef CLIONPROGRAM_BASE_H
#define CLIONPROGRAM_BASE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

struct Packet {
    int packetID, sf, rssi;
    unsigned long long timeStamp;
    double snr;

    explicit Packet(const string &s);
};

struct Record {
    unsigned long long timeStamp;
    pair<double, double> location;
    
    explicit Record(const string &s);
};

class PacketGroup {
private:
    int packetID;
    vector<Packet> packets;
    pair<double, double> location;
    unsigned long long timeStampAvg;

public:
    explicit PacketGroup(int packetID);
    void addPacket(const Packet &newPacket);
    bool isAbnormal();
    void updateTimeStamp();
    int getPacketID() const;
    unsigned long long getTimeStampAvg() const;
    void setLocation(const pair<double, double> &loc);
    pair<double, double> getLocation() const;
};

void readInput(const string &RECEIVED_FILE_NAME, const string &RECORD_FILE_NAME, vector<PacketGroup> &ret);

void printOutput(const string &OUTPUT_FILE_NAME, const vector<PacketGroup> &packetGroups);

void splitByChar(const string &s, vector<string> &ret, const char splitChar);

#endif //CLIONPROGRAM_BASE_H
