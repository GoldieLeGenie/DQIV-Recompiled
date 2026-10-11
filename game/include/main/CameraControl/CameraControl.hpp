#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"

struct CameraControl {
    struct SeqCameraControl {
        int frame;
        dss::Fix32Vector3 pos;
        dss::Fix32Vector3 pos_a;
        dss::Fix32Vector3 ang;
        dss::Fix32Vector3 ang_a;
        dss::Fix32Vector3 posEnd;
        dss::Fix32Vector3 angEnd;
    };
    DataObject data_;
    dss::Fix32Vector3 iniPosition_;
    dss::Vector3<short> iniAngle_;
    unsigned int maxSeqPhase_;
    unsigned int seqPhase_;
    short dt_;
    short waitCounter_;
    SeqCameraControl *seqData_;
    char camera_name[128];
    int wait_;
    CameraControl();
    ~CameraControl();
    void terminate();
    void initCameraControl(dss::Fix32Vector3 position, dss::Vector3<short> angle);
    void readCameraData(const char* fname, int arg);
    bool calc(dss::Fix32Vector3& arg_position, dss::Vector3<short>& arg_angle);
    bool moveCamera(dss::Fix32Vector3& arg_position, dss::Vector3<short>& arg_angle);
};
