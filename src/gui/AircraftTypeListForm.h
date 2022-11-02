#pragma once

#include <QDialog>

namespace Ui {
    class aircraft_type_list_form;
};

class QAbstractItemView;
class QAbstractItemModel;

class AircraftTypeListForm : public QDialog {
public:
    AircraftTypeListForm( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~AircraftTypeListForm();

    void setModel( QAbstractItemModel* typesMod );

    void viewToolButtons( bool isVisible );

private slots:
    void addType();
    void editType();
    void delType();

    void refreshTypes();

signals:
    void addAircraftType( long long idParent );
    void editAircraftType( long long id );
    void delAircraftType( long long id );

    void refreshAircraftModel( QAbstractItemView* );
private:
    Ui::aircraft_type_list_form* _UI;

private:
    Q_OBJECT
};
