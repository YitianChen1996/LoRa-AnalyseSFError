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
#include <iomanip>


using namespace std;

struct Globals {
    const vector<double> sfSendTime = {0.103, 0.17, 0.296, 0.52, 1.013, 2.002};
    // rssi is (-107) - (-73) -> (3) -> (37)
    const int rssiOffset = 110;
    const int snrOffset = 25;
};

class Packet {
private:
    int packetID, sf, rssi;
    unsigned long long timeStamp;
    double snr;

    void setContentInternal(const string &s);

public:
    explicit Packet(const string &s);

    Packet();

    void setContent(const string &s);

    int getSf() const;

    int getRssi() const;
};

void readInput(const string &RECEIVED_FILE_NAME,
               const string &RECORD_FILE_NAME,
               vector<vector<double>> &ret,
               Globals *pGlobal);

void printOutput(const string &OUTPUT_FILE_NAME, const vector<vector<double>> &errorRate, Globals *pGlobal);

void splitByChar(const string &s, vector<string> &ret, const char splitChar);

#endif //CLIONPROGRAM_BASE_H
