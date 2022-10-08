#pragma once

#include <QObject>
#include <QWidget>

#include <memory>

class QSettings;

class OpenCVDatabase;
class dbLoader;
class dbWriter;

using std::unique_ptr;
using std::shared_ptr;

class OpenCVCore : public QObject {
public:
    bool GUIConnect(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());

    OpenCVDatabase* getDb() const { return m_Db; }
    shared_ptr< dbLoader > getDbLoader() const;
    shared_ptr< dbWriter > getDbWriter() const;

    void loadImage( const QImage& im, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );

private slots:
    void saveImageToDb( const QImage& im, QString imName );

signals:
    void setWidget( QWidget* w );

private:
    OpenCVCore( OpenCVDatabase* db, QObject* parent = nullptr );
    virtual ~OpenCVCore();

    OpenCVDatabase* m_Db;
    shared_ptr< dbLoader > m_databaseLoader;
    shared_ptr< dbWriter > m_databaseWriter;
    QSettings* m_cvSettings;

    friend class dbOpenCvS;
private:
    Q_OBJECT
};
