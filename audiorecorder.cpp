#include "audiorecorder.h"
#include <QDebug>
#include <QThread>

AudioRecorder::AudioRecorder(QObject *parent) : QObject(parent)
{
    _isRecordered = false;
    _file.setFileName("output.wav");


    _recorder = new QAudioRecorder(this);

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/wav");
    audioSettings.setQuality(QMultimedia::HighQuality);

    _recorder->setAudioSettings(audioSettings);
    _recorder->setOutputLocation(QUrl(_file.fileName()));

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/wav");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

    _output = new QAudioOutput(format, this);
    _input = new QAudioInput(format, this);


}

void AudioRecorder::record()
{
    qDebug() << "Started";
    //_recorder->record();

//    _file.open( QIODevice::ReadWrite );
//    qDebug() << _file.error();
//    if (_file.isOpen()) {
//        qDebug() << "It's ok";
//    }


//    _input->start(&_file);
//    qDebug() << _input->error();
//    _output->start(&_file);

    _buffer.open(QIODevice::ReadWrite);
    if(!_isRecordered)
        _input->start(&_buffer);
    else
        _output->start(&_buffer);
}

void AudioRecorder::stop()
{
    _isRecordered = !_isRecordered;
    _buffer.close();
    qDebug() << "Stoped";
    _recorder->stop();
    _output->stop();
    _input->stop();

    _file.close();
}
