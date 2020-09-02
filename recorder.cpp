#include "recorder.h"
#include <QDebug>

Recorder::Recorder(QObject * parent) : QObject(parent)
{
    //Init audio settings
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/wav");
    audioSettings.setQuality(QMultimedia::HighQuality);

    //Set settings and output location
    _recorder.setEncodingSettings(audioSettings);
    _recorder.setOutputLocation(QUrl::fromLocalFile(OUTPUT_NAME));
}

//Start recording and save data to output file
void Recorder::record()
{
    _recorder.record();
}

//Pause recording
void Recorder::pause()
{
    _recorder.pause();
}

//Stop recording and exit
void Recorder::stop()
{
    _recorder.stop();
    thread()->quit();
}
