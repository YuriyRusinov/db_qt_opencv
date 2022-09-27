#pragma once

#include <QObject>
#include <QWidget>

class OpenCVDatabase;

class OpenCVCore : public QObject {
public:
    bool GUIConnect(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());

private:
    OpenCVCore( OpenCVDatabase* db, QObject* parent = nullptr );
    virtual ~OpenCVCore();

    OpenCVDatabase* m_Db;

    friend class dbOpenCvS;
private:
    Q_OBJECT
};
