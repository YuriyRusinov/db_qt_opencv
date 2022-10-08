#include <QDialog>
#include <QSettings>
#include <QtDebug>
#include <dbLoginForm.h>

#include <dbLoader.h>
#include <dbWriter.h>
#include <opencv_database.h>
#include <cvImageForm.h>
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

shared_ptr<dbLoader> OpenCVCore::getDbLoader() const {
    return m_databaseLoader ;
}

shared_ptr< dbWriter > OpenCVCore::getDbWriter() const {
    return m_databaseWriter ;
}

void OpenCVCore::loadImage( const QImage& im, QWidget* parent, Qt::WindowFlags flags ) {
    cvImageForm* imWidget = new cvImageForm(im, parent, flags);
    connect( imWidget, &cvImageForm::saveImage, this, &OpenCVCore::saveImageToDb );
    emit setWidget( imWidget );
}

void OpenCVCore::saveImageToDb( const QImage& im, QString imName ) {
    int imId = m_databaseWriter->insertImage( im, imName );
    qDebug() << __PRETTY_FUNCTION__ << imId;
}
