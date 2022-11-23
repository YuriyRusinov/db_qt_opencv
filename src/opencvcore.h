#pragma once

#include <QObject>
#include <QWidget>

#include <memory>

class QAbstractItemModel;
class QSettings;
class QAbstractItemView;

class OpenCVDatabase;
class dbLoader;
class dbWriter;
class AircraftType;
class AircraftImage;

using std::unique_ptr;
using std::shared_ptr;

class OpenCVCore : public QObject {
public:
    bool GUIConnect( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    QWidget* GUIViewImages( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    QWidget* GUIViewTypes( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );

    OpenCVDatabase* getDb() const { return m_Db; }
    shared_ptr< dbLoader > getDbLoader() const;
    shared_ptr< dbWriter > getDbWriter() const;

    void loadAircraftImage( shared_ptr< AircraftImage > image, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    void loadImage( long long id=-1, QString name=QString(), const QImage& im=QImage(), QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    void dbDisconnect();

private slots:
    void saveImageToDb( shared_ptr< AircraftImage> aircraftImage );
    void insertImageToDb( );
    void updateImageInDb( qlonglong id );
    void deleteImageFromDb( qlonglong id );

    void refreshModel( );

    void insertType( long long idParent );
    void updateType( long long idType );
    void deleteType( long long idType );

    void refreshTypes( QAbstractItemView* tvTypes );

    void setParentType( shared_ptr< AircraftType > aircType );
    void saveType( shared_ptr< AircraftType > aircType );
    void setImageType( shared_ptr< AircraftImage > aircraftImage );

signals:
    void setWidget( QWidget* w );
    void setAircraftType( shared_ptr< AircraftType > aircType );

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
