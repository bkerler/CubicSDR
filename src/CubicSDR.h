#pragma once

//WX_GL_CORE_PROFILE 1
//WX_GL_MAJOR_VERSION 3
//WX_GL_MINOR_VERSION 2

#include <thread>

#include "wx/glcanvas.h"
#include "PrimaryGLContext.h"

#include "ThreadQueue.h"
#include "SDRThread.h"
#include "SDRPostThread.h"
#include "AudioThread.h"
#include "DemodulatorMgr.h"

#define NUM_DEMODULATORS 1

class CubicSDR: public wxApp {
public:
    CubicSDR() :
    m_glContext(NULL), frequency(DEFAULT_FREQ), sdrThread(NULL), sdrPostThread(NULL), threadCmdQueueSDR(NULL), iqVisualQueue(NULL), iqPostDataQueue(NULL), audioVisualQueue(NULL), t_SDR(NULL),  t_PostSDR(NULL), sampleRate(DEFAULT_SAMPLE_RATE) {

    }

    PrimaryGLContext &GetContext(wxGLCanvas *canvas);

    virtual bool OnInit();
    virtual int OnExit();

    void setFrequency(long long freq);
    long long getFrequency();

    void setOffset(long long ofs);
    long long getOffset();

    void setSampleRate(long long rate_in);
    long long getSampleRate();

    std::vector<SDRDeviceInfo *> *getDevices();
    void setDevice(int deviceId);
    int getDevice();

    DemodulatorThreadOutputQueue* getAudioVisualQueue();
    DemodulatorThreadInputQueue* getIQVisualQueue();
    DemodulatorMgr &getDemodMgr();

    void bindDemodulator(DemodulatorInstance *demod);
    void removeDemodulator(DemodulatorInstance *demod);

private:
    PrimaryGLContext *m_glContext;
    std::vector<SDRDeviceInfo *> devs;

    DemodulatorMgr demodMgr;

    long long frequency;
    long long offset;
    long long sampleRate;

    SDRThread *sdrThread;
    SDRPostThread *sdrPostThread;

    SDRThreadCommandQueue* threadCmdQueueSDR;
    SDRThreadIQDataQueue* iqPostDataQueue;
    DemodulatorThreadInputQueue* iqVisualQueue;
    DemodulatorThreadOutputQueue* audioVisualQueue;

    std::thread *t_SDR;
    std::thread *t_PostSDR;
};

DECLARE_APP(CubicSDR)
