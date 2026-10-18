#pragma once
#include "globaldefs.h"

// Residents of the immigrant town: envoy type and map position of each one
struct UnkImmigrantTown {
    int count_;                                 // 0x00
    unsigned char type_[42];                    // 0x04
    unsigned char x_[42];                       // 0x2E (0xff: not placed)
    unsigned char y_[42];                       // 0x58
    int queueCount_;                            // 0x84
    int queueFlag_;                             // 0x88
    unsigned char queueX_[20];                  // 0x8C positions waiting for a resident
    unsigned char queueY_[20];                  // 0xA0

    static UnkImmigrantTown* getSingleton();
    void rebuildResidents();                    // rebuild the resident list from the envoys
    void resetPositions();                    // clear the positions
    void addResident(int type);
    int hasResidentType(int type);
    void placeNextResident(int x, int y);
    void placePriorityResident(int x, int y, int count, int* types);
    int getResidentPoseAt(int x, int y);
    int getResidentIndexAt(int x, int y);
    int getTypePose(int type);
    void placeQueuedResidents();
    int matchesResidentCategory(int index, int type);
    int getTownLevel();                     // town level from the area flags
};
