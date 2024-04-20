#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QTimer>

enum FREQ_BAND{
    ALPHA,
    BETA,
    DELTA,
    THETA,
    GAMMA
};

struct WaveForm{
    float frequency;
    float amplitude;
};


class Headset : public QObject
{
    Q_OBJECT
public:
    explicit Headset(int nodes, QObject *parent = nullptr);
    void applyTreatment(float freq);
    void applyTreatment(int site, float freq);
    QVector<WaveForm> getSiteWaveForms(FREQ_BAND bandOmitted);

    // Getters
    int getNumNodes() { return numNodes; }


private:
    int numNodes;
    WaveForm generateWaveForm(FREQ_BAND band);
    float randomFloat(float min, float max);
    float elapsedTime;
    float treatmentFreq;

private slots:
    void applyTreatmentToCurrNode();

};
#endif // HEADSET_H
