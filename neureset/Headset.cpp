#include "Headset.h"

#include<cstdlib>
#include <QDebug>
#include <QTimer>
using namespace std;

Headset::Headset(int nodes, QObject *parent) : QObject(parent), numNodes(nodes){
    //numNodes  = nodes;
}

void Headset::applyTreatment(float freq){
//    currentNodeIndex = 0;
    elapsedTime = 0;
//    if (currentNodeIndex >= numNodes)
//    {
//        return;
//    }
    if (elapsedTime >= 1000) return;

    treatmentFreq = freq;

    applyTreatmentToCurrNode();
}

void Headset::applyTreatmentToCurrNode(){
//    if (currentNodeIndex >= numNodes) {
//        return;
//    }
    if (elapsedTime >= 1000){
        qDebug()<<"round  complete";
        return;
    }
    qDebug() << "Applying treatment of" << treatmentFreq << "to nodes";
    //currentNodeIndex++;
    elapsedTime+=62;
    QTimer::singleShot(62, this, &Headset::applyTreatmentToCurrNode);
}

//int Headset::readEEGBaseline(int site){

//}

QVector<float> Headset::readBase(){
    //gets the frequency for a specific wave length (for testing)
    return readBaseDelta();
}

QVector<QVector<float>> Headset::getDomFreq(){
    //get vals for freq and amps at a point in time (freqs change every 1 second)
    qDebug("getting dominant freq");
    //change the function to get a different dominant freequency
    QVector<QVector<float>> out;
    out << readBaseAlpha() << readBaseBeta() << readBaseDelta() << readBaseTheta();

    return out;
}

QVector<QVector<QVector<float>>> Headset::readAllFreqs(){
    //returns a qvecor of each of the recorded bands, each haaving the freq,amp pairs of each node
    QVector<QVector<QVector<float>>> out;
    out << readBaseAlphaAll() << readBaseBetaAll() << readBaseDeltaAll() << readBaseThetaAll();
    return out;
}

//getting the dominant frequency is finding which one of these is dominant in the brain (corresponds to different states)
QVector<float> Headset::readBaseAlpha(){
    QVector<float> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 4) + 8;      //gives range 8-12
//    }
    float freq;
    float amp;
    freq = (rand() % 3) + 8;      //gives range 8-12
    freq += (float(rand())/float((RAND_MAX)));
    freq = (freq * 100 + 0.5) / 100;
    qDebug() <<"Alpha freq: "<< freq;
    out  << freq;
    amp = (float(rand())/float((RAND_MAX))) * 10 + 20;  //gives values from 20 - 30
    qDebug() <<"Alpha amp: "<< amp;
    out << amp;

//    out << (rand() % 4) + 8;      //gives range 8-12
//    out << 1;


    return out;
}

QVector<float> Headset::readBaseBeta(){
    QVector<float> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 18) + 12;      //gives range 12-30
//    }
    float freq;
    float amp;
    freq = (rand() % 18) + 12;      //gives range 12-30
    freq += (float(rand())/float((RAND_MAX)));
    freq = (freq * 100 + 0.5) / 100;
    qDebug() <<"Beta freq: "<< freq;
    out  << freq;
    amp = (float(rand())/float((RAND_MAX))) * 10 + 10;  //gives values from 10 - 20
    qDebug() <<"Beta amp: "<< amp;
    out << amp;

//    out << (rand() % 18) + 12;      //gives range 12-30
//    out << 1;


    return out;
}

QVector<float> Headset::readBaseDelta(){
    QVector<float> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 3) + 1;      //gives range 1-4
//    }

    float freq;
    float amp;
    freq = (rand() % 3) + 1;      //gives range 1-4
    freq += (float(rand())/float((RAND_MAX)));
    freq = (freq * 100 + 0.5) / 100;
    qDebug() <<"Delta freq: "<< freq;
    out  << freq;
    amp = (float(rand())/float((RAND_MAX))) * 10 + 40;  //gives values from 40 - 50
    qDebug() <<"Delta amp: "<< amp;
    out << amp;

//    out << (rand() % 3) + 1;      //gives range 1-4
//    out << 1;
    return out;
}

QVector<float> Headset::readBaseTheta(){
    QVector<float> out;

//    for (int i = 0; i < numNodes; i++){
//        out[i] = (rand() % 4) + 4;      //gives range 4-8
//    }
    float freq;
    float amp;
    freq = (rand() % 4) + 4;      //gives range 4-8
    freq += (float(rand())/float((RAND_MAX)));
    freq = (freq * 100 + 0.5) / 100;
    qDebug() <<"Theta freq: "<< freq;
    out  << freq;
    amp = (float(rand())/float((RAND_MAX))) * 10 + 30;  //gives values from 30 - 40
    qDebug() <<"Theta amp: "<< amp;
    out << amp;

    out << (rand() % 4) + 4;      //gives range 4-8
    out << 1;
    return out;
}

// slots
//void Headset::readBaselineSlot(){

//}


//getting the dominant frequency is finding which one of these is dominant in the brain (corresponds to different states)
QVector<QVector<float>> Headset::readBaseAlphaAll(){
    QVector<QVector<float>> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
    float freq;
    float amp;
    QVector<float> pair;
    for (int i = 0; i < numNodes; i++){
        freq = (rand() % 3) + 8;      //gives range 8-12
        freq += (float(rand())/float((RAND_MAX)));
        freq = (freq * 100 + 0.5) / 100;
        qDebug() <<"freq: "<< freq;
        pair.push_back(freq);
        amp = (float(rand())/float((RAND_MAX))) * 10 + 20;  //gives values from 40 - 50
        qDebug() <<"amp: "<< amp;
        pair.push_back(amp);
        out.push_back(pair);
        pair.clear();
    }

    return out;
}

QVector<QVector<float>> Headset::readBaseBetaAll(){
    QVector<QVector<float>> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
    float freq;
    float amp;
    QVector<float> pair;
    for (int i = 0; i < numNodes; i++){
        freq = (rand() % 18) + 12;      //gives range 12-30
        freq += (float(rand())/float((RAND_MAX)));
        freq = (freq * 100 + 0.5) / 100;
        qDebug() <<"freq: "<< freq;
        pair.push_back(freq);

        amp = (float(rand())/float((RAND_MAX))) * 10 + 10;  //gives values from 40 - 50
        qDebug() <<"amp: "<< amp;
        pair.push_back(amp);
        out.push_back(pair);
        pair.clear();
    }

    return out;
}

QVector<QVector<float>> Headset::readBaseDeltaAll(){
    QVector<QVector<float>> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
    float freq;
    float amp;
    QVector<float> pair;
    for (int i = 0; i < numNodes; i++){
        freq = (rand() % 3) + 1;
        freq += (float(rand())/float((RAND_MAX)));
        freq = (freq * 100 + 0.5) / 100;
        qDebug() <<"freq: "<< freq;
        pair.push_back(freq);
        amp = (float(rand())/float((RAND_MAX))) * 10 + 40;  //gives values from 40 - 50
        qDebug() <<"amp: "<< amp;
        pair.push_back(amp);
        out.push_back(pair);
        pair.clear();
    }

    return out;
}

QVector<QVector<float>> Headset::readBaseThetaAll(){
    QVector<QVector<float>> out;
    //since it sounds like the implementation is up to our interpretation, individual node values might not be necessary
    //if it is, use the loop, need to add the amplitude after the values are generated (not sure what the expected values look like)
    float freq;
    float amp;
    QVector<float> pair;
    for (int i = 0; i < numNodes; i++){
        freq = (rand() % 4) + 4;      //gives range 4-8
        freq += (float(rand())/float((RAND_MAX)));
        freq = (freq * 100 + 0.5) / 100;
        qDebug() <<"freq: "<< freq;
        pair.push_back(freq);
        amp = (float(rand())/float((RAND_MAX))) * 10 + 30;  //gives values from 40 - 50
        qDebug() <<"amp: "<< amp;
        pair.push_back(amp);
        out.push_back(pair);
        pair.clear();
    }

    return out;
}


