#include <QDialog>
#include <QSettings>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QtDebug>

#include <dbLoginForm.h>
#include <dbLoader.h>
#include <dbWriter.h>
#include <opencv_database.h>
#include <cvImageForm.h>
#include <cvImageListForm.h>
#include <cvImageModel.h>
#include <AircraftTypeListForm.h>
#include <AircraftTypeModel.h>
#include <AircraftTypeForm.h>
#include "opencvcore.h"

using std::make_unique;
using std::make_shared;

bool OpenCVCore::GUIConnect(QWidget* parent, Qt::WindowFlags flags) {
    qDebug() << __PRETTY_FUNCTION__;
    dbLoginForm* dbConnForm = new dbLoginForm( parent, flags );
    m_cvSettings->beginGroup("System Settings");
    QString iUser = m_cvSettings->value("Username").toString();
    QString iDatabase = m_cvSettings->value("Database").toString();
    QString iServer = m_cvSettings->value("Server").toString();
    bool retVal = false;
    int port = m_cvSettings->value("Port", QString::number(5432)).toInt();
    dbConnForm->setUserName( iUser );
    dbConnForm->setDatabase( iDatabase );
    dbConnForm->setIpServer( iServer );
    dbConnForm->setDbPort( port );
    if( dbConnForm != nullptr && dbConnForm->exec() == QDialog::Accepted ) {
        QString user = dbConnForm->getUserName();
        QString password = dbConnForm->getPassword();
        QString dataBase = dbConnForm->getDatabase();
        QString server = dbConnForm->getIpServer();
        int port = dbConnForm->getDbPort();
        qDebug() << __PRETTY_FUNCTION__ << user << password << dataBase << server << port;
        retVal = m_Db->connect( server.toStdString(), dataBase.toStdString(), user.toStdString(), password.toStdString(), port );
        qDebug() << __PRETTY_FUNCTION__ << retVal;
        if( retVal ) {
            m_cvSettings->setValue("Username", user);
            m_cvSettings->setValue("Database", dataBase);
            m_cvSettings->setValue("Server", server);
            m_cvSettings->setValue("Port", port);
        }
    }
    if( dbConnForm )
        delete dbConnForm;
    m_cvSettings->endGroup();
    return retVal;
}

OpenCVCore::OpenCVCore( OpenCVDatabase* db, QObject* parent )
    : QObject( parent ),
    m_Db(db), 
    m_databaseLoader ( make_shared<dbLoader>( db ) ),
    m_databaseWriter ( make_shared<dbWriter>( db ) ),
    m_cvSettings( new QSettings(QSettings::NativeFormat, QSettings::UserScope, tr("YRusinov")) ) {
}

OpenCVCore::~OpenCVCore() {
    m_Db->disconnect();
    delete m_cvSettings;
}

shared_ptr< dbLoader > OpenCVCore::getDbLoader() const {
    return m_databaseLoader ;
}

shared_ptr< dbWriter > OpenCVCore::getDbWriter() const {
    return m_databaseWriter ;
}

void OpenCVCore::loadImage( long long id, QString name, const QImage& im, QWidget* parent, Qt::WindowFlags flags ) {
    cvImageForm* imWidget = new cvImageForm(id, name, im, parent, flags);
    connect( imWidget, &cvImageForm::saveImage, this, &OpenCVCore::saveImageToDb );
    emit setWidget( imWidget );
}

void OpenCVCore::saveImageToDb( const QImage& im, QString imName, qlonglong id ) {
    qlonglong imId;
    if( id < 0 ) {
        imId = m_databaseWriter->insertImage( im, imName );
    }
    else {
        imId = m_databaseWriter->updateImage( im, imName, id);
    }
    qDebug() << __PRETTY_FUNCTION__ << imId;
}

QWidget* OpenCVCore::GUIViewImages( QWidget* parent, Qt::WindowFlags flags ) {
    shared_ptr< dbLoader > dbl = m_databaseLoader;
    QMap<long long, shared_ptr<AircraftImage>> images = dbl->loadImages();
    cvImageListForm* imListForm = new cvImageListForm( parent, flags );

    connect( imListForm, &cvImageListForm::insertImage, this, &OpenCVCore::insertImageToDb );
    connect( imListForm, &cvImageListForm::updateImage, this, &OpenCVCore::updateImageInDb );
    connect( imListForm, &cvImageListForm::deleteImage, this, &OpenCVCore::deleteImageFromDb );
    connect( imListForm, &cvImageListForm::refreshModel, this, &OpenCVCore::refreshModel );

    QAbstractItemModel * imModel = new cvImageModel( images );
    imListForm->setImagesModel( imModel );
    qDebug() << __PRETTY_FUNCTION__ << imModel->rowCount();
    emit setWidget( imListForm );
    return imListForm;
}

void OpenCVCore::insertImageToDb( ) {
    qDebug() << __PRETTY_FUNCTION__;
    loadImage( );
}

void OpenCVCore::updateImageInDb( qlonglong id ) {
    qDebug() << __PRETTY_FUNCTION__ << id;
    shared_ptr< AircraftImage > wim = m_databaseLoader->loadImage(id);
    loadImage( id, wim->getName(), wim->getImage() );
}

void OpenCVCore::deleteImageFromDb( qlonglong id ) {
    long long idd = m_databaseWriter->delImage( id );
    qDebug() << __PRETTY_FUNCTION__ << id << idd;
}

void OpenCVCore::refreshModel( ) {
    cvImageListForm* cvForm = qobject_cast< cvImageListForm* >(this->sender());
    if( cvForm == nullptr )
        return;
    qDebug() << __PRETTY_FUNCTION__;
    QMap<long long, shared_ptr<AircraftImage>> images = m_databaseLoader->loadImages();
    QAbstractItemModel * imModel = new cvImageModel( images );
    cvForm->setImagesModel( imModel );
}

void OpenCVCore::dbDisconnect() {
    if( m_Db != nullptr && m_Db->connected() )
        m_Db->disconnect();
}

QWidget* OpenCVCore::GUIViewTypes( QWidget* parent, Qt::WindowFlags flags ) {
    qDebug() << __PRETTY_FUNCTION__;
    map<long long, shared_ptr<AircraftType>> aircraftTypes = m_databaseLoader->loadTypes();
    AircraftTypeListForm* airTypesForm = new AircraftTypeListForm( parent, flags );

    connect( airTypesForm, &AircraftTypeListForm::addAircraftType, this, &OpenCVCore::insertType );
    connect( airTypesForm, &AircraftTypeListForm::editAircraftType, this, &OpenCVCore::updateType );
    connect( airTypesForm, &AircraftTypeListForm::delAircraftType, this, &OpenCVCore::deleteType );
    connect( airTypesForm, &AircraftTypeListForm::refreshAircraftModel, this, &OpenCVCore::refreshTypes );

    airTypesForm->viewToolButtons( true );
    AircraftTypeModel* aTypesMod = new AircraftTypeModel( aircraftTypes );
    airTypesForm->setModel( aTypesMod );
    emit setWidget( airTypesForm );

    return airTypesForm;
}

void OpenCVCore::insertType( long long idParent ) {
    shared_ptr< AircraftType > pType = m_databaseLoader->loadType( idParent );
    shared_ptr< AircraftType > wType( new AircraftType );
    wType->setParent( pType );
    qDebug() << __PRETTY_FUNCTION__ << idParent << wType.get() << pType.get();
    AircraftTypeForm* tForm = new AircraftTypeForm( wType );
    connect( tForm, &AircraftTypeForm::selectParentType, this, &OpenCVCore::setParentType );
    connect( tForm, &AircraftTypeForm::saveTypeToDb, this, &OpenCVCore::saveType );
    emit setWidget( tForm );
}

void OpenCVCore::updateType( long long idType ) {
    qDebug() << __PRETTY_FUNCTION__ << idType;
    shared_ptr< AircraftType > wType = m_databaseLoader->loadType( idType );
    AircraftTypeForm* tForm = new AircraftTypeForm( wType );
    connect( tForm, &AircraftTypeForm::selectParentType, this, &OpenCVCore::setParentType );
    connect( tForm, &AircraftTypeForm::saveTypeToDb, this, &OpenCVCore::saveType );
    emit setWidget( tForm );
}

void OpenCVCore::deleteType( long long idType ) {
    qDebug() << __PRETTY_FUNCTION__ << idType;
    m_databaseWriter->delType( idType );
}

void OpenCVCore::refreshTypes( QAbstractItemView* tvTypes ) {
    if( tvTypes == nullptr )
        return;
    map<long long, shared_ptr<AircraftType>> aircraftTypes = m_databaseLoader->loadTypes();
    AircraftTypeModel* aTypesMod = new AircraftTypeModel( aircraftTypes );
    QAbstractItemModel* oldModel = tvTypes->model();
    tvTypes->setModel( aTypesMod );
    if( oldModel )
        delete oldModel;
}

void OpenCVCore::setParentType( shared_ptr< AircraftType > airCraftType ) {
    if( airCraftType == nullptr )
        return;
    long long idType = airCraftType->getId();
    qDebug() << __PRETTY_FUNCTION__ << idType;
    map<long long, shared_ptr<AircraftType>> aircraftTypes = m_databaseLoader->loadTypes();
    AircraftTypeListForm* airTypesForm = new AircraftTypeListForm( );

    airTypesForm->viewToolButtons( false );
    AircraftTypeModel* aTypesMod = new AircraftTypeModel( aircraftTypes );
    airTypesForm->setModel( aTypesMod );
    if( airTypesForm->exec() == QDialog::Accepted ) {
        AircraftType* aType = airTypesForm->getType();
        if( aType && aType->getId() != airCraftType->getId() ) {
            shared_ptr< AircraftType > asType ( aType );
            airCraftType->setParent( asType );
        }
    }
    delete airTypesForm;
}

void OpenCVCore::saveType( shared_ptr< AircraftType > airCraftType ) {
    if( airCraftType == nullptr )
        return;
    long long idType = airCraftType->getId();
    qDebug() << __PRETTY_FUNCTION__ << idType;
    long long idType0;
    if( idType < 0 ) {
        idType0 = m_databaseWriter->insertType( airCraftType );
    }
    else {
        idType0 = m_databaseWriter->updateType( airCraftType );
    }
}

