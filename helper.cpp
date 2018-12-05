//
// Created by 64419 on 11/26/2018.
//

#include "base.h"

void countErrorBetweenTwoPackets(const Packet &prevPacket, const Packet &curPacket, vector<vector<int>> &errorNum,
                                 vector<vector<int>> &totalNum, Globals *pGlobal) {
    int prevSf = prevPacket.getSf(), curSf = curPacket.getSf();
    int missingNum = (curSf > prevSf) ? curSf - prevSf - 1 : curSf + 6 - prevSf - 1;
    if (missingNum == 0) { return; }
    double rssiDiff = (double) (curPacket.getRssi() - prevPacket.getRssi()) / (double) (missingNum + 1);
    for (int base = 1; base <= missingNum; base++) {
        int sfIndex = (prevSf + base - 7) % 6;
        double estimatedRssi = ((double) prevPacket.getRssi() + ((double) base) * rssiDiff);
        int rssiIndex = estimatedRssi + pGlobal->rssiOffset;
        errorNum[sfIndex][rssiIndex]++;
        totalNum[sfIndex][rssiIndex]++;
    }
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
    if (!receivedFile.is_open() || !recordFile.is_open()) {
        cout << "file open failed" << endl;
        return;
    }
    
    vector<double> temp;
    temp.assign(40, 0);
    ret.assign(6, temp);
    vector<vector<int>> totalNum, errorNum;
    vector<int> temp2;
    temp2.assign(40, 0);
    totalNum.assign(6, temp2);
    errorNum.assign(6, temp2);
    
    string line;
    getline(receivedFile, line); // read out the first line.

    Packet curPacket, prevPacket;
    while (getline(receivedFile, line)) {
        curPacket.setContent(line);
        int sfIndex = curPacket.getSf() - 7;
        int rssiIndex = curPacket.getRssi() + (double) pGlobal->rssiOffset;
        totalNum[sfIndex][rssiIndex]++;
        if (prevPacket.getSf() != 0) {
            countErrorBetweenTwoPackets(prevPacket, curPacket, errorNum, totalNum, pGlobal);
        }
        prevPacket = curPacket;
    }

    for (int i = 0; i < totalNum.size(); i++) {
        for (int j = 0; j < totalNum[i].size(); j++) {
            ret[i][j] = (totalNum[i][j] == 0) ? 0 : (double) errorNum[i][j] / (double) totalNum[i][j];
        }
    }
    
    receivedFile.close();
    recordFile.close();
}

void printOutput(const string &OUTPUT_FILE_NAME, const vector<vector<double>> &errorRate, Globals *pGlobal) {
    ofstream outputFile(OUTPUT_FILE_NAME);
    for (int i = 0; i < errorRate.size(); i++) {
        for (int j = 0; j < errorRate[i].size(); j++) {
            outputFile << setprecision(5) << setw(7) << errorRate[i][j] << ",";
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