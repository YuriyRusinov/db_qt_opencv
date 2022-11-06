#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
    class aircraft_type_form;
};

using std::shared_ptr;

class AircraftType;

class AircraftTypeForm : public QDialog {
public:
    AircraftTypeForm( shared_ptr< AircraftType > aircType=nullptr, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~AircraftTypeForm();

    shared_ptr< AircraftType > getType() const;
    void setType( shared_ptr< AircraftType > aircType );

private slots:
    void setParentType();
    void clearParentType();

    void saveType();
    void close();

signals:
    void saveTypeToDb( shared_ptr< AircraftType > aircType );
    void selectParentType( shared_ptr< AircraftType > aircType );

private:
    Ui::aircraft_type_form* _UI;
    shared_ptr< AircraftType > m_aircraftType;

private:
    Q_OBJECT
};
