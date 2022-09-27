#include <QDialog>
#include <QtDebug>
#include <dbLoginForm.h>

#include <opencv_database.h>
#include "opencvcore.h"

bool OpenCVCore::GUIConnect(QWidget* parent, Qt::WindowFlags flags) {
    qDebug() << __PRETTY_FUNCTION__;
    dbLoginForm* dbConnForm = new dbLoginForm( parent, flags );
    if( dbConnForm != nullptr && dbConnForm->exec() == QDialog::Accepted ) {
        QString user = dbConnForm->getUserName();
        QString password = dbConnForm->getPassword();
        QString dataBase = dbConnForm->getDatabase();
        QString server = dbConnForm->getIpServer();
        int port = dbConnForm->getDbPort();
        qDebug() << __PRETTY_FUNCTION__ << user << password << dataBase << server << port;
        bool retVal = m_Db->connect( server.toStdString(), dataBase.toStdString(), user.toStdString(), password.toStdString(), port );
        qDebug() << __PRETTY_FUNCTION__ << retVal;
    }
    if( dbConnForm )
        delete dbConnForm;
    return false;
}

OpenCVCore::OpenCVCore( OpenCVDatabase* db, QObject* parent )
    : QObject( parent ), m_Db(db) {}

OpenCVCore::~OpenCVCore() {
}
