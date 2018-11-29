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
#include <memory>

using namespace std;

struct Globals {
    const vector<double> sfSendTime = {0.103, 0.17, 0.296, 0.52, 1.013, 2.002};
    const int rssiOffset = 110;
    const int snrOffset = 25;
};

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
    double rssiAvg;
    Globals *globals;

public:
    PacketGroup(int packetID, Globals *globals);
    
    void addPacket(const Packet &newPacket);
    
    bool isAbnormal();
    
    void updateTimeStamp();
    
    void updateRssiAvg();
    
    double getRssiAvg();
    
    int getPacketID() const;
    
    unsigned long long getTimeStampAvg() const;
    
    void setLocation(const pair<double, double> &loc);
    
    pair<double, double> getLocation() const;
};

void readInput(const string &RECEIVED_FILE_NAME,
               const string &RECORD_FILE_NAME,
               vector<double> &ret,
               Globals *pGlobal);

void printOutput(const string &OUTPUT_FILE_NAME, const vector<double> &errorRate, Globals *pGlobal);

void splitByChar(const string &s, vector<string> &ret, const char splitChar);

#endif //CLIONPROGRAM_BASE_H
