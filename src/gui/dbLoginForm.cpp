#include "dbLoginForm.h"
#include "ui_db_login_form.h"

dbLoginForm::dbLoginForm(QWidget* parent, Qt::WindowFlags flags)
    : QDialog( parent, flags),
    _UI( new Ui::db_login_form ) {
    _UI->setupUi(this);
}

dbLoginForm::~dbLoginForm() {
    delete _UI;
}

QString dbLoginForm::getUserName() const {
    return _UI->lEUsername->text();
}

void dbLoginForm::setUserName( QString username ) {
    _UI->lEUsername->setText( username );
}

QString dbLoginForm::getPassword() const {
    return _UI->lEPassword->text();
}

QString dbLoginForm::getDatabase() const {
    return _UI->lEDatabase->text();
}

void dbLoginForm::setDatabase( QString db ) {
    _UI->lEDatabase->setText( db );
}

QString dbLoginForm::getIpServer() const {
    return _UI->lEServer->text();
}

void dbLoginForm::setIpServer( QString ip ) {
    _UI->lEServer->setText( ip );
}

int dbLoginForm::getDbPort() const {
    return _UI->lEPort->text().toInt();
}

void dbLoginForm::setDbPort(int port) {
    _UI->lEPort->setText( QString::number( port ) );
}
