#pragma once

#include <QObject>
#include <QWidget>

class QSettings;

class OpenCVDatabase;

class OpenCVCore : public QObject {
public:
    bool GUIConnect(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());

private:
    OpenCVCore( OpenCVDatabase* db, QObject* parent = nullptr );
    virtual ~OpenCVCore();

    OpenCVDatabase* m_Db;

    QSettings* m_cvSettings;

    friend class dbOpenCvS;
private:
    Q_OBJECT
};
