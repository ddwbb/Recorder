#ifndef RECORDER_H
#define RECORDER_H

#include <QUrl>
#include <QObject>
#include <QThread>
#include <QAudioRecorder>

#define OUTPUT_NAME "output.wav"

class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(QObject *parent = nullptr);

signals:

public slots:
    void stop();
    void pause();
    void record();

private:
    QAudioRecorder _recorder;
};

#endif // RECORDER_H
