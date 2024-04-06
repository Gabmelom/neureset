#ifndef HEADSET_H
#define HEADSET_H

#include <QObject>
#include <QVector>


class Headset : public QObject
{
    Q_OBJECT
public:
    explicit Headset(QObject *parent = nullptr);

    void applyTreatment(int site, int baseline);
    int readEEGBaseline(int site);
    QVector<float> readBaseline();

public slots:
    void readBaselineSlot();

};
#endif // HEADSET_H
