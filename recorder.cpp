#include "recorder.h"
#include <QDebug>

Recorder::Recorder(QObject * parent) : QObject(parent)
{
    _file.setFileName("/output.raw");
    _file.open(QFile::WriteOnly);
    if (!_file.isOpen()) {
        QMessageBox msg(QMessageBox::Warning, "Ошибка", "Ошибка открытия файла для записи");
        thread()->exit();
    }
}

void Recorder::write(QByteArray * buffer)
{
    _file.write(buffer->data());
    _file.flush();
}

void Recorder::stop()
{
    thread()->quit();
}
