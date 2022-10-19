#include <QDialog>
#include <QSettings>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QtDebug>
#include <dbLoginForm.h>

#include <dbLoader.h>
#include <dbWriter.h>
#include <opencv_database.h>
#include <cvImageForm.h>
#include <cvImageListForm.h>
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
    QMap<long, dbImages> images = dbl->loadImages();
    cvImageListForm* imListForm = new cvImageListForm( parent, flags );

    connect( imListForm, &cvImageListForm::insertImage, this, &OpenCVCore::insertImageToDb );
    connect( imListForm, &cvImageListForm::updateImage, this, &OpenCVCore::updateImageInDb );
    connect( imListForm, &cvImageListForm::deleteImage, this, &OpenCVCore::deleteImageFromDb );
    connect( imListForm, &cvImageListForm::refreshModel, this, &OpenCVCore::refreshModel );

    QAbstractItemModel * imModel = new QStandardItemModel( images.size(), 3 );
    imListForm->setImagesModel( imModel );
    imModel->setHeaderData( 0, Qt::Horizontal, tr("ID"), Qt::DisplayRole );
    imModel->setHeaderData( 1, Qt::Horizontal, tr("Image name"), Qt::DisplayRole );
    imModel->setHeaderData( 2, Qt::Horizontal, tr("Image"), Qt::DisplayRole );
    int i=0;
    for(QMap<long, dbImages>::const_iterator pim = images.constBegin();
                                             pim != images.constEnd();
                                             pim++ ) {
        imModel->setData( imModel->index(i, 0), QVariant((qlonglong)pim.key()), Qt::DisplayRole );
        imModel->setData( imModel->index(i, 0), QVariant((qlonglong)pim.key()), Qt::UserRole );
        imModel->setData( imModel->index(i, 1), pim.value().getName(), Qt::DisplayRole );
        QImage im = pim.value().getImage();
        imModel->setData( imModel->index(i, 2), QVariant(im), Qt::DecorationRole );
        i++;
    }
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
    dbImages wim = m_databaseLoader->loadImage(id);
    loadImage( id, wim.getName(), wim.getImage() );
}

void OpenCVCore::deleteImageFromDb( qlonglong id ) {
    long long idd = m_databaseWriter->delImage( id );
    qDebug() << __PRETTY_FUNCTION__ << id << idd;
}

void OpenCVCore::refreshModel( QAbstractItemModel* imModel ) {
    if( imModel == nullptr )
        return;
    qDebug() << __PRETTY_FUNCTION__;
}
