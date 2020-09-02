#ifndef RECORDER_H
#define RECORDER_H

#include <QByteArray>
#include <QFile>
#include <QObject>
#include <QThread>
#include <QMessageBox>

class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(QObject *parent = nullptr);

signals:

public slots:
    void write(QByteArray *);
    void stop();

private:

    QFile _file;
};

#endif // RECORDER_H
