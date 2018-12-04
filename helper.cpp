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

int getIndexLatitude(double latitude) {
    int diff = (int) ((42.092444 - latitude) * 1000000.0);
    int index = diff / (10000 / DIVIDE_LAT_TO);
    return (index >= 0 && index <= DIVIDE_LAT_TO - 1) ? index : -1;
}

int getIndexLongitude(double longitude) {
    int diff = (int) ((longitude - (-75.980282)) * 1000000.0);
    int index = diff / ((10000 / DIVIDE_LAT_TO) * 2);
    return (index >= 0 && index <= DIVIDE_LNG_TO - 1) ? index : -1;
}

/**
 * Return ONLY abnormal packets!
 * @param RECEIVED_FILE_NAME
 * @param RECORD_FILE_NAME
 * @param ret abnormal packets
 */
void readInput(const string &RECEIVED_FILE_NAME,
               const string &RECORD_FILE_NAME,
               vector<vector<double>> &ret,
               Globals *pGlobal) {
    ifstream receivedFile(RECEIVED_FILE_NAME);
    ifstream recordFile(RECORD_FILE_NAME);
    
    vector<vector<int>> total, error;
    vector<PacketGroup> temp;
    vector<int> realTemp;
    vector<double> realTemp2;
    realTemp.assign(DIVIDE_LNG_TO, 0);
    realTemp2.assign(DIVIDE_LNG_TO, 0);
    total.assign(DIVIDE_LAT_TO, realTemp);
    error.assign(DIVIDE_LAT_TO, realTemp);
    ret.assign(DIVIDE_LAT_TO, realTemp2);
    
    if (!receivedFile.is_open() || !recordFile.is_open()) {
        cout << "file open failed!" << endl;
        return;
    }
    
    string line;
    while (getline(receivedFile, line)) {
        Packet packet(line);
        if (temp.empty()) {
            temp.emplace_back(packet.packetID, pGlobal);
            temp.back().addPacket(packet);
        } else if (temp.back().getPacketID() == packet.packetID) {
            temp.back().addPacket(packet);
        } else {
            fillLocation(temp.back(), recordFile);
            int row = getIndexLatitude(temp.back().getLocation().first);
            int col = getIndexLongitude(temp.back().getLocation().second);
            if (row != -1 && col != -1) {
                if (temp.back().isAbnormal()) { error[row][col]++; }
                total[row][col]++;
            }
            temp.pop_back();
            temp.emplace_back(packet.packetID, pGlobal);
            temp.back().addPacket(packet);
        }
    }
    
    for (int i = 0; i < DIVIDE_LAT_TO; i++) {
        for (int j = 0; j < DIVIDE_LNG_TO; j++) {
            ret[i][j] = (total[i][j] == 0) ? -1 : (double) error[i][j] / (double) total[i][j];
        }
    }
    
    receivedFile.close();
    recordFile.close();
}

void printOutput(const string &OUTPUT_FILE_NAME, const vector<vector<double>> &errorRate, Globals *pGlobal) {
    ofstream outputFile(OUTPUT_FILE_NAME);
    for (int i = 0; i < errorRate.size(); i++) {
        for (int j = 0; j < errorRate[i].size(); j++) {
            outputFile << setw(7) << setprecision(3) << errorRate[i][j] << " ";
        }
        outputFile << endl;
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