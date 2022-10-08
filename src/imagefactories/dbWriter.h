#pragma once

#include <QObject>

class OpenCVDatabase;

class dbWriter : public QObject {
public:
    explicit dbWriter( OpenCVDatabase* db, QObject* parent=nullptr );
    dbWriter( const dbWriter& ) = delete;
    dbWriter& operator=( const dbWriter& ) = delete;
    virtual ~dbWriter( );

    int insertImage(const QImage& im, QString imName);

private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
