#include "Headset.h"

#include<cstdlib>
#include <QDebug>
#include <QTimer>
using namespace std;

Headset::Headset(int nodes, QObject *parent) : QObject(parent), numNodes(nodes)
{
}

void Headset::applyTreatment(float freq){
    elapsedTime = 0;
    if (elapsedTime >= 1000) return;

    treatmentFreq = freq;

    applyTreatmentToCurrNode();
}

void Headset::applyTreatmentToCurrNode(){
    if (elapsedTime >= 1000){
        return;
    }
    qDebug() << "Applying treatment of" << treatmentFreq << "Hz to patient";
    elapsedTime+=63;
    QTimer::singleShot(63, this, &Headset::applyTreatmentToCurrNode);
}

// Generates a wave form for 4 out of 5 possible frequency bands
QVector<WaveForm> Headset::getSiteWaveForms(FREQ_BAND bandOmitted){

    QVector<WaveForm> waveForms;
    for (int i = 0; i < 5; i++){
        if (i == bandOmitted) continue;
        WaveForm waveForm = generateWaveForm((FREQ_BAND)i);
        waveForms.push_back(waveForm);
    }

    return waveForms;
}


WaveForm Headset::generateWaveForm(FREQ_BAND band){
    WaveForm waveform;
    float frequency;
    float amplitude;
    switch (band){
        case ALPHA:
            frequency = randomFloat(8, 12);     // 8-12 Hz
            amplitude = randomFloat(20, 30);    // 20-30 mV
            break;
        case BETA:
            frequency = randomFloat(12, 30);    // 12-30 Hz
            amplitude = randomFloat(10, 20);    // 10-20 mV
            break;
        case DELTA:
            frequency = randomFloat(1, 4);      // 1-4 Hz
            amplitude = randomFloat(40, 50);    // 40-50 mV
            break;
        case THETA:
            frequency = randomFloat(4, 8);      // 4-8 Hz
            amplitude = randomFloat(30, 40);    // 30-40 mV
            break;
        case GAMMA:
            frequency = randomFloat(30, 100);   // 30-100 Hz
            amplitude = randomFloat(5, 10);     // 5-10 mV
            break;
        default:
            qInfo() << "Invalid frequency band";
            frequency = 0;
            amplitude = 0;
            break;
    }
    
    waveform.frequency = frequency;
    waveform.amplitude = amplitude;
    return waveform;
}


float Headset::randomFloat(float min, float max){
    return (float(rand())/float((RAND_MAX))) * (max - min) + min;
}