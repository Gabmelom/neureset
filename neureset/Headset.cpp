#include "Headset.h"

#include<cstdlib>
#include <QDebug>
#include <QTimer>
using namespace std;

Headset::Headset(int nodes, QObject *parent) : QObject(parent), numNodes(nodes){
    //numNodes  = nodes;
}

void Headset::applyTreatment(int freq){
    currentNodeIndex = 0;

    if (currentNodeIndex >= numNodes)
    {
        return;
    }

    treatmentFreq = freq;

    applyTreatmentToCurrNode();
}

void Headset::applyTreatmentToCurrNode(){
    if (currentNodeIndex >= numNodes) {
        return;
    }
    qDebug() << "Applying treatment of" << treatmentFreq << "to node" << currentNodeIndex + 1;
    currentNodeIndex++;
    QTimer::singleShot(150, this, &Headset::applyTreatmentToCurrNode);
}

//int Headset::readEEGBaseline(int site){

//}

QVector<int> Headset::readBase(){
    //gets the frequency for a specific wave length (for testing)
    return readBaseDelta();
}

QVector<QVector<int>> Headset::getDomFreq(){
    //get vals for freq and amps at a point in time (freqs change every 1 second)
    qDebug("getting dominant freq");
    //change the function to get a different dominant freequency
    QVector<QVector<int>> out;
    out << readBaseAlpha() << readBaseBeta() << readBaseDelta() << readBaseTheta();
//    out[0] = readBaseAlpha();
//    out[1] = readBaseBeta();
//    out[2] = readBaseDelta();
//    out[3] = readBaseTheta();
//    thread->wait(1000);

    return out;
}


//getting the dominant frequency is finding which one of these is dominant in the brain (corresponds to different states)
QVector<int> Headset::readBaseAlpha(){
    QVector<int> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 4) + 8;      //gives range 8-12
//    }
    out << (rand() % 4) + 8;      //gives range 8-12
    out << 1;


    return out;
}

QVector<int> Headset::readBaseBeta(){
    QVector<int> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 18) + 12;      //gives range 12-30
//    }
    out << (rand() % 18) + 12;      //gives range 12-30
    out << 1;


    return out;
}

QVector<int> Headset::readBaseDelta(){
    QVector<int> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 3) + 1;      //gives range 1-4
//    }
    out << (rand() % 3) + 1;      //gives range 1-4
    out << 1;
    return out;
}

QVector<int> Headset::readBaseTheta(){
    QVector<int> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 4) + 4;      //gives range 4-8
//    }
    out << (rand() % 4) + 4;      //gives range 4-8
    out << 1;
    return out;
}

// slots
//void Headset::readBaselineSlot(){

//}




