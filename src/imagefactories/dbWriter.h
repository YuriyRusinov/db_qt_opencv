#pragma once

#include <QObject>

#include <memory>

using std::shared_ptr;

class OpenCVDatabase;
class AircraftType;

class dbWriter : public QObject {
public:
    explicit dbWriter( OpenCVDatabase* db, QObject* parent=nullptr );
    dbWriter( const dbWriter& ) = delete;
    dbWriter& operator=( const dbWriter& ) = delete;
    virtual ~dbWriter( );

    long long insertImage(const QImage& im, QString imName);
    long long updateImage(const QImage& im, QString imName, qlonglong id);
    long long delImage( qlonglong id );

    long long insertType( shared_ptr< AircraftType > wType );
    long long updateType( shared_ptr< AircraftType > wType );
    long long delType( long long id );
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
