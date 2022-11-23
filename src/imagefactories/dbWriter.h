#pragma once

#include <QObject>

#include <memory>

using std::shared_ptr;

class OpenCVDatabase;
class AircraftType;
class AircraftImage;

class dbWriter : public QObject {
public:
    explicit dbWriter( OpenCVDatabase* db, QObject* parent=nullptr );
    dbWriter( const dbWriter& ) = delete;
    dbWriter& operator=( const dbWriter& ) = delete;
    virtual ~dbWriter( );

    long long insertAircraftImage( shared_ptr< AircraftImage > aircImage );
    long long updateAircraftImage( shared_ptr< AircraftImage > aircImage );
    long long delImage( qlonglong id );

    long long insertType( shared_ptr< AircraftType > wType );
    long long updateType( shared_ptr< AircraftType > wType );
    long long delType( long long id );

private:
    //
    // Functions for insert image to database
    //
    long long insertImage(const QImage& im, QString imName);
    long long updateImage(const QImage& im, QString imName, qlonglong id);

private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
