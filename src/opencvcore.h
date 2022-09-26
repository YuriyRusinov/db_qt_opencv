#pragma once

#include <QObject>
#include <QWidget>

class OpenCVCore : public QObject {
public:
    bool GUIConnect(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());

private:
    OpenCVCore( QObject* parent = nullptr );
    virtual ~OpenCVCore();

    friend class dbOpenCvS;
private:
    Q_OBJECT
};
