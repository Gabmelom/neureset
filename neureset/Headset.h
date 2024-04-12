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
    void applyTreatment(int freq);
//    int readEEGBaseline(int site);
    QVector<int> readBase();
    void readBaselineSlot();
    QVector<QVector<int>> getDomFreq();



//public slots:
//    void readBaselineSlot();

private:
    int numNodes;
    QVector<int> readBaseAlpha();   //probablyy just the freq amp overall, not for each node
    QVector<int> readBaseBeta();
    QVector<int> readBaseDelta();
    QVector<int> readBaseTheta();

    int currentNodeIndex;
    int treatmentFreq;

private slots:
    void applyTreatmentToCurrNode();

};
#endif // HEADSET_H
