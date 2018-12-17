//
// Created by 64419 on 11/26/2018.
//

#include "base.h"

Packet::Packet(const string &s) {
    vector<string> temp;
    splitByChar(s, temp, ',');
    timeStamp = stoull(temp[0], nullptr, 10);
    packetID = stoi(temp[1], nullptr, 10);
    sf = stoi(temp[2], nullptr, 10);
    rssi = stoi(temp[3], nullptr, 10);
    snr = stod(temp[4], nullptr);
}

int PacketGroup::getPacketID() const { return packetID; }

PacketGroup::PacketGroup(int packetID, Globals *globals) {
    this->packetID = packetID;
    this->globals = globals;
}

unsigned long long PacketGroup::getTimeStampAvg() const { return timeStampAvg; }

void PacketGroup::addPacket(const Packet &newPacket) { packets.emplace_back(newPacket); }

void PacketGroup::setLocation(const pair<double, double> &loc) { this->location = loc; }

pair<double, double> PacketGroup::getLocation() const { return location; }

bool PacketGroup::isAbnormal() {
    const int higher = 10, lower = 9;
    unordered_set<int> temp;
    for (const Packet &packet : packets) { temp.insert(packet.sf); }
    if (temp.find(8) != temp.end() && temp.find(16) == temp.end()) { return true; }
    return false;
}

bool PacketGroup::isNormal() {
    const int higher = 10, lower = 9;
    unordered_set<int> temp;
    for (const Packet &packet : packets) { temp.insert(packet.sf); }
    if (temp.find(8) != temp.end()) { return true; }
    return false;
}

void PacketGroup::updateTimeStamp() {
    unsigned long long temp = 0;
    for (const Packet &packet : packets) {
        temp += packet.timeStamp;
    }
    timeStampAvg = temp / packets.size();
}

void PacketGroup::updateRssiAvg() {
    double temp = 0;
    for (Packet &packet : packets) { temp += (double) packet.rssi; }
    rssiAvg = temp / (double) packets.size();
}

double PacketGroup::getSnrAvg() {
    double temp = 0;
    for (Packet &packet : packets) { temp += (double) packet.snr; }
    return temp / (double) packets.size();
}

double PacketGroup::getRssiAvg() {return rssiAvg; }