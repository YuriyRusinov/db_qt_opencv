#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
    class aircraft_type_form;
};

using std::shared_ptr;

class AircraftType;

class AirCraftTypeForm : public QDialog {
public:
    AirCraftTypeForm( shared_ptr< AircraftType > aircType=nullptr, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~AirCraftTypeForm();

    shared_ptr< AircraftType > getType() const;

private slots:
    void setParentType();
    void clearParentType();

private:
    Ui::aircraft_type_form* _UI;
    shared_ptr< AircraftType > m_aircraftType;

private:
    Q_OBJECT
};
