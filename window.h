#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QByteArray>
#include <QThread>

#include "recorder.h"

#define BUFFER_SIZE 8192

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void readData();

signals:
    void recordStarted(QByteArray *);
    void recordStoped();

private:
    Ui::Window * _ui;

    QByteArray * _buffer;

    QIODevice * _inputDevice;
    QIODevice * _outputDevice;

    QAudioInput * _inputAudio;
    QAudioOutput * _outputAudio;

    QAudioDeviceInfo _inputInfo;
    QAudioDeviceInfo _outputInfo;

    QThread * _recorderThread;

    Recorder * _recorder;

    bool _isActive;

    void startRecord();
    void stopRecord();
};
#endif // WINDOW_H
