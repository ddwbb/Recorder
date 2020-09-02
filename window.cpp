#include "window.h"
#include "ui_window.h"
#include <QDebug>

Window::Window(QWidget *parent)
    : QMainWindow(parent),
      _ui(new Ui::Window),
      _inputInfo(QAudioDeviceInfo::defaultInputDevice()),
      _outputInfo(QAudioDeviceInfo::defaultOutputDevice()),
      _isActive(false)
{
    _ui->setupUi(this);

    _buffer = new QByteArray(BUFFER_SIZE, 0);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::UnSignedInt );
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    if (!_inputInfo.isFormatSupported(format))
        format = _inputInfo.nearestFormat(format);

    _inputAudio = new QAudioInput(_inputInfo, format, this);

    if (!_outputInfo.isFormatSupported(format))
        format = _outputInfo.nearestFormat(format);

    _outputAudio = new QAudioOutput(_outputInfo, format, this);

    _inputDevice = nullptr;
    _outputDevice = nullptr;

    _recorder = new Recorder;
    _recorderThread = new QThread(this);

    _recorder->moveToThread(_recorderThread);
    _recorderThread->start();


    connect(this, &Window::recordStarted, _recorder, &Recorder::write);
    connect(this, &Window::recordStoped, _recorder, &Recorder::stop);
}

Window::~Window()
{
    delete _ui;
    delete _buffer;

    delete _inputAudio;
    delete _outputAudio;
}


void Window::on_startButton_clicked()
{
    if (!_isActive) {
        startRecord();
        connect(_inputDevice, &QIODevice::readyRead, this, &Window::readData);
        _ui->startButton->setText("Пауза");
    } else {
        stopRecord();
        disconnect(_inputDevice, 0, this, 0);
        _ui->startButton->setText("Продолжить");
    }
    _isActive = !_isActive;
}

void Window::on_stopButton_clicked()
{
    stopRecord();
    emit recordStoped();
    _recorderThread->wait();
    this->close();
}

void Window::readData()
{
    if (!_inputAudio)
        return;

    quint64 size = _inputAudio->bytesReady();

    _inputDevice->read(_buffer->data(), size);
    emit recordStarted(_buffer);
    if (size > 0)
        _outputDevice->write(_buffer->data(), size);
}

void Window::startRecord()
{
    _inputDevice = _inputAudio->start();
    _outputDevice = _outputAudio->start();
}

void Window::stopRecord()
{
    _inputAudio->stop();
    _outputAudio->stop();
}
