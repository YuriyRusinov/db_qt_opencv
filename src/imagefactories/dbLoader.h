#pragma once

#include <QMap>
#include <QObject>

#include <map>
#include <memory>
#include <string>

#include <AircraftImage.h>
#include <AircraftType.h>
#include <opencv2/core/mat.hpp>

using std::shared_ptr;
using std::make_shared;
using std::map;
using std::string;

class OpenCVDatabase;

class dbLoader : public QObject {
public:
    explicit dbLoader( OpenCVDatabase* db, QObject* parent = nullptr );
    dbLoader() = delete;
    dbLoader( const dbLoader& ) = delete;
    dbLoader& operator=( const dbLoader& dbl ) = delete;
    virtual ~dbLoader();

    QMap< long long, shared_ptr< AircraftImage > > loadImages() const;
    shared_ptr< AircraftImage > loadImage( qlonglong id ) const;
    cv::Mat loadCVImage( qlonglong id, string& imName ) const;

    map< long long, shared_ptr< AircraftType > > loadTypes() const;
    shared_ptr< AircraftType > loadType( long long id ) const;

    friend class dbOpenCvS;
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
