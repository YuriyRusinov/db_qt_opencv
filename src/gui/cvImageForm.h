#pragma once

#include <memory>
#include <QImage>
#include <QWidget>

namespace Ui {
    class cv_image_form;
};

class AircraftImage;
class AircraftType;

using std::shared_ptr;

class cvImageForm : public QWidget {
public:
    cvImageForm( shared_ptr< AircraftImage > aImage, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~cvImageForm();

private slots:
    void loadImageFromFile();
    void saveImageToDb();
    void classifyImage();
    void setImageType( shared_ptr< AircraftType > aType );
    void clearType();
    void setAirType() {};

signals:
    void saveImage( shared_ptr<AircraftImage> );
    void setAircraftType( shared_ptr< AircraftImage > );

private:
    //
    //
    //
    friend class OpenCVCore;

    Ui::cv_image_form* _UI;
    shared_ptr< AircraftImage > m_aircraftImage;
    qlonglong m_id;
    QString m_ImageName;
    QImage m_Image;

private:
    Q_OBJECT
};
