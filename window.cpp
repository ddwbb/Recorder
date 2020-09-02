#include "window.h"
#include "ui_window.h"
#include <QDebug>

Window::Window(QWidget *parent)
    : QMainWindow(parent),
      _ui(new Ui::Window),
      _inputInfo(QAudioDeviceInfo::defaultInputDevice()),
      _outputInfo(QAudioDeviceInfo::defaultOutputDevice()),
      _isActive(false),
      _isPaused(false)
{
    _ui->setupUi(this);
    _buffer = new QByteArray(BUFFER_SIZE, 0);

    //Init audio foramt
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::UnSignedInt );
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/wav");

    //Checking for support format by input device
    if (!_inputInfo.isFormatSupported(format))
        format = _inputInfo.nearestFormat(format);

    //Init audio input
    _inputAudio = new QAudioInput(_inputInfo, format, this);

    //Checking for support format by output device
    if (!_outputInfo.isFormatSupported(format))
        format = _outputInfo.nearestFormat(format);

    //Init audio output
    _outputAudio = new QAudioOutput(_outputInfo, format, this);

    _inputDevice = nullptr;
    _outputDevice = nullptr;

    //Init audio recorder and thread
    _recorder = new Recorder;
    _recorderThread = new QThread(this);

    //Move recorder to another thread
    _recorder->moveToThread(_recorderThread);
    _recorderThread->start();

    //Connect signals with recorder
    connect(this, &Window::recordStoped, _recorder, &Recorder::stop);
    connect(this, &Window::recordPaused, _recorder, &Recorder::pause);
    connect(this, &Window::recordStarted, _recorder, &Recorder::record);
}


Window::~Window()
{
    delete _ui;
    delete _buffer;

    delete _inputAudio;
    delete _outputAudio;
}

//StartButton click event
void Window::on_startButton_clicked()
{
    //Is first click?
    if (!_isActive) {
        startRecord();
        _isActive = true;
        _ui->startButton->setText("Пауза");
        return;
    }

    //What should I do: pause or resume?
    if (!_isPaused) {
        pauseRecord();
        _ui->startButton->setText("Продолжить");
    } else {
        resumeRecord();
        _ui->startButton->setText("Пауза");
    }
    _isPaused = !_isPaused;
}

//StopButton click event
void Window::on_stopButton_clicked()
{
    close();
}

//Read data from input device to buffer and write it to output device
void Window::readData()
{
    if (!_inputAudio)
        return;

    quint64 size = _inputAudio->bytesReady();

    if (size > BUFFER_SIZE)
        size = BUFFER_SIZE;

    _inputDevice->read(_buffer->data(), size);
    _outputDevice->write(_buffer->data(), size);
}

//Stop record
void Window::stopRecord()
{
    _inputAudio->stop();
    _outputAudio->stop();

    //send signal to Recorder to stop
    emit recordStoped();
}

//Start recording
void Window::startRecord()
{
    _inputDevice = _inputAudio->start();
    _outputDevice = _outputAudio->start();
    //when input data is ready for read we have to read it
    connect(_inputDevice, &QIODevice::readyRead, this, &Window::readData);

    //send signal to Recorder to start
    emit recordStarted();
}

//Pause recording
void Window::pauseRecord()
{
    _inputAudio->suspend();
    _outputAudio->suspend();

    //send signal to Recorder to pause
    emit recordPaused();
}

//Resume recording
void Window::resumeRecord()
{
    _inputAudio->resume();
    _outputAudio->resume();

    //send signal to Recorder to resume
    emit recordStarted();
}


void Window::closeEvent(QCloseEvent *) {
    stopRecord();
    //wait until recorder save data and shut down
    _recorderThread->wait();
}
