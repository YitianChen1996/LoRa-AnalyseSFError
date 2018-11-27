//
// Created by 64419 on 11/26/2018.
//

#include "base.h"

Record::Record(const string &s) {
    vector<string> temp;
    splitByChar(s, temp, ',');
    timeStamp = stoull(temp[0], nullptr, 10);
    location.first = stod(temp[2], nullptr);
    location.second = stod(temp[3], nullptr);
}