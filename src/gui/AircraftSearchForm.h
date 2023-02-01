#pragma once

#include <QDialog>
#include <QImage>

namespace Ui {
    class aircraft_search_form;
};

class AircraftSearchForm : public QDialog {
public:
    AircraftSearchForm( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~AircraftSearchForm();

private slots:
    void loadAircraftImage();

private:
    Ui::aircraft_search_form* _UI;
    QImage m_SearchImage;

private:
    Q_OBJECT
};
