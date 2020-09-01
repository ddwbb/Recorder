#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "audiorecorder.h"

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

signals:
    void recordStarted();
    void recordStoped();

private:
    Ui::Window *ui;

    AudioRecorder _recorder;
};
#endif // WINDOW_H
