#pragma once

#include <memory>
#include <QImage>
#include <QWidget>

namespace Ui {
    class cv_image_form;
};

class AircraftImage;

using std::shared_ptr;

class cvImageForm : public QWidget {
public:
    cvImageForm( qlonglong id=-1, const QString& imName=QString(), const QImage& image=QImage(), QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    cvImageForm( shared_ptr< AircraftImage > aImage, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~cvImageForm();

private slots:
    void loadImageFromFile();
    void saveImageToDb();
    void classifyImage();

signals:
    void saveImage(const QImage&, QString, qlonglong);
    void classifyImage( shared_ptr< AircraftImage > );

private:
    Ui::cv_image_form* _UI;
    shared_ptr< AircraftImage > m_aircraftImage;
    qlonglong m_id;
    QString m_ImageName;
    QImage m_Image;

private:
    Q_OBJECT
};
