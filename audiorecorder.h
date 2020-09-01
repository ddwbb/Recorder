#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QUrl>
#include <QFile>
#include <QObject>
#include <QBuffer>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioRecorder>



class AudioRecorder : public QObject {
    Q_OBJECT

    QAudioRecorder * _recorder;
    QAudioOutput * _output;
    QAudioInput * _input;

    QAudioDeviceInfo * _inputInfo;
    QAudioDeviceInfo * _outputInfo;

    QIODevice * _inputDevice;
    QIODevice * _outputDevice;

    QBuffer _buffer;

    QFile _file;

    bool _isRecordered;

public:
    explicit AudioRecorder(QObject * parent = nullptr);

signals:

public slots:
    void record();
    void stop();
};

#endif // AUDIORECORDER_H
