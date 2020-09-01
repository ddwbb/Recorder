#include "window.h"
#include "ui_window.h"
#include <QDebug>

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    connect(this, &Window::recordStarted, &_recorder, &AudioRecorder::record);
    connect(this, &Window::recordStoped, &_recorder, &AudioRecorder::stop);
}

Window::~Window()
{
    delete ui;
}


void Window::on_startButton_clicked()
{

    emit recordStarted();
}

void Window::on_stopButton_clicked()
{

    _recorder.stop();
}
