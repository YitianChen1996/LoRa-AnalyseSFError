//
// Created by 64419 on 11/26/2018.
//

#include "base.h"


/**
 * Return ONLY abnormal packets!
 * @param RECEIVED_FILE_NAME
 * @param RECORD_FILE_NAME
 * @param ret abnormal packets
 */
void readInput(const string &RECEIVED_FILE_NAME,
               const string &RECORD_FILE_NAME,
               vector<double> &ret,
               Globals *pGlobal) {
    ifstream receivedFile(RECEIVED_FILE_NAME);
    ifstream recordFile(RECORD_FILE_NAME);
    
    ret.assign(40, 0);
    vector<int> total, error;
    total.assign(40, 0);
    error.assign(40, 0);
    
    if (!receivedFile.is_open() || !recordFile.is_open()) {
        cout << "file open failed!" << endl;
        return;
    }
    
    vector<PacketGroup> temp;
    int count = 0;
    
    string line;
    while (getline(receivedFile, line)) {
        Packet packet(line);
        if (temp.empty()) {
            temp.emplace_back(packet.packetID, pGlobal);
            temp.back().addPacket(packet);
        } else if (temp.back().getPacketID() == packet.packetID) {
            temp.back().addPacket(packet);
        } else {
            temp.back().updateRssiAvg();
            total[(int) temp.back().getRssiAvg() + pGlobal->rssiOffset]++;
            if (temp.back().isAbnormal()) {
                error[(int) temp.back().getRssiAvg() + pGlobal->rssiOffset]++;
            }
            count++;
            temp.pop_back();
            temp.emplace_back(packet.packetID, pGlobal);
            temp.back().addPacket(packet);
        }
    }
    
    for (int i = 0; i < 40; i++) {
        if (total[i] == 0) { continue; }
        ret[i] = (double) error[i] / (double) total[i];
    }
    
    receivedFile.close();
    recordFile.close();
}

void printOutput(const string &OUTPUT_FILE_NAME, const vector<double> &errorRate, Globals *pGlobal) {
    ofstream outputFile(OUTPUT_FILE_NAME);
    for (int i = 0; i < errorRate.size(); i++) {
        outputFile << i - pGlobal->rssiOffset << "," << errorRate[i] << endl;
    }
    outputFile.close();
}

void splitByChar(const string &s, vector<string> &ret, const char splitChar) {
    ret.clear();
    size_t curp = 0, nextp = 0;
    while (true) {
        nextp = s.find(splitChar, curp);
        if (nextp == string::npos) { break; }
        ret.push_back(s.substr(curp, nextp - curp));
        curp = nextp + 1;
    }
    ret.push_back(s.substr(curp, s.length() - curp));
}