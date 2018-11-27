#include "base.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Arguments error" << endl;
        return 0;
    }
    const string RECEIVED_FILE_NAME = string(argv[1]);
    const string RECORD_FILE_NAME = string(argv[2]);
    const string OUTPUT_FILE_NAME = string(argv[3]);
    std::cout << "Hello, World!" << std::endl;
    vector<PacketGroup> packetGroups;
    readInput(RECEIVED_FILE_NAME, RECORD_FILE_NAME, packetGroups);
    printOutput(OUTPUT_FILE_NAME, packetGroups);
    
    return 0;
}