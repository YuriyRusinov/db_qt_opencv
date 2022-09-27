#pragma once

#include <QDialog>

namespace Ui {
    class db_login_form;
};

class dbLoginForm : public QDialog {
public:
    dbLoginForm(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~dbLoginForm();

    QString getUserName() const;
    void setUserName( QString username );

    QString getPassword() const;

    QString getDatabase() const;
    void setDatabase( QString db );

    QString getIpServer() const;
    void setIpServer( QString ip );

    int getDbPort() const;
    void setDbPort(int port);

private:
    Ui::db_login_form* _UI;

private:
    Q_OBJECT
};
