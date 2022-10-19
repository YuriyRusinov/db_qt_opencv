#pragma once

#include <QObject>

class OpenCVDatabase;

class dbWriter : public QObject {
public:
    explicit dbWriter( OpenCVDatabase* db, QObject* parent=nullptr );
    dbWriter( const dbWriter& ) = delete;
    dbWriter& operator=( const dbWriter& ) = delete;
    virtual ~dbWriter( );

    long long insertImage(const QImage& im, QString imName);
    long long updateImage(const QImage& im, QString imName, qlonglong id);
    long long delImage( qlonglong id );

private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
