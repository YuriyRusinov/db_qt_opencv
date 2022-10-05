#pragma once

#include <QObject>

class OpenCVDatabase;

class dbWriter : public QObject {
public:
    explicit dbWriter( OpenCVDatabase* db, QObject* parent=nullptr );
    dbWriter( const dbWriter& ) = delete;
    dbWriter& operator=( const dbWriter& ) = delete;
    virtual ~dbWriter( );

    bool insertImage(const QImage& im);

private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
