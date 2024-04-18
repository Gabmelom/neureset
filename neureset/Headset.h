#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QTimer>



class Headset : public QObject
{
    Q_OBJECT
public:
    explicit Headset(int nodes, QObject *parent = nullptr);

//    void applyTreatment(int site, int baseline);
    void applyTreatment(float freq);
//    int readEEGBaseline(int site);
    QVector<float> readBase();
    void readBaselineSlot();
    QVector<QVector<float>> getDomFreq();

    QVector<QVector<QVector<float>>> readAllFreqs();    //this wouldn't be used in DFC, but would be useful ffor wave form creation

//public slots:
//    void readBaselineSlot();

private:
    int numNodes;
    QVector<float> readBaseAlpha();   //probablyy just the freq amp overall, not for each node
    QVector<float> readBaseBeta();
    QVector<float> readBaseDelta();
    QVector<float> readBaseTheta();
    //return a list  of length NODES of  frequency amplitude readings
    QVector<QVector<float>> readBaseAlphaAll();
    QVector<QVector<float>> readBaseBetaAll();
    QVector<QVector<float>> readBaseDeltaAll();
    QVector<QVector<float>> readBaseThetaAll();

    //int currentNodeIndex;
    float elapsedTime;
    float treatmentFreq;

private slots:
    void applyTreatmentToCurrNode();

};
#endif // HEADSET_H
