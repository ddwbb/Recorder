#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QByteArray>
#include <QCloseEvent>
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
    void recordPaused();
    void recordStoped();
    void recordStarted();

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

    //Object to record input audio data and save it to file
    Recorder * _recorder;

    bool _isActive;
    bool _isPaused;

    void stopRecord();
    void startRecord();
    void pauseRecord();
    void resumeRecord();

    void closeEvent(QCloseEvent *);
};
#endif // WINDOW_H
