//
// Created by 64419 on 11/26/2018.
//

#include "base.h"

void fillLocation(PacketGroup &packetGroup, ifstream &recordFile) {
    packetGroup.updateTimeStamp();
    string line;
    while (getline(recordFile, line)) {
        Record record(line);
        if (record.timeStamp >= packetGroup.getTimeStampAvg()) {
            packetGroup.setLocation(record.location);
            break;
        }
    }
}

/**
 * Return ONLY abnormal packets!
 * @param RECEIVED_FILE_NAME
 * @param RECORD_FILE_NAME
 * @param ret abnormal packets
 */
void readInput(const string &RECEIVED_FILE_NAME, const string &RECORD_FILE_NAME, vector<PacketGroup> &ret) {
    ifstream receivedFile(RECEIVED_FILE_NAME);
    ifstream recordFile(RECORD_FILE_NAME);
    if (!receivedFile.is_open() || !recordFile.is_open()) {
        cout << "file open failed!" << endl;
        return;
    }
    
    string line;
    while (getline(receivedFile, line)) {
        Packet packet(line);
        if (ret.empty()) {
            ret.emplace_back(PacketGroup(packet.packetID));
            ret.back().addPacket(packet);
        } else if (ret.back().getPacketID() != packet.packetID) {
            // pop out normal packet group.
            if (!ret.back().isAbnormal()) { ret.pop_back(); }
            else { fillLocation(ret.back(), recordFile); }
            ret.emplace_back(PacketGroup(packet.packetID));
            ret.back().addPacket(packet);
        } else {
            ret.back().addPacket(packet);
        }
    }
    
    if (!ret.back().isAbnormal()) { ret.pop_back(); }
    else { fillLocation(ret.back(), recordFile); }
    
    receivedFile.close();
    recordFile.close();
}

void printOutput(const string &OUTPUT_FILE_NAME, const vector<PacketGroup> &packetGroups) {
    ofstream outputFile(OUTPUT_FILE_NAME);
    for (const PacketGroup &packetGroup : packetGroups) {
        outputFile << packetGroup.getLocation().first << " " << packetGroup.getLocation().second << " "
                   << packetGroup.getPacketID() << " " << packetGroup.getTimeStampAvg() << endl;
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