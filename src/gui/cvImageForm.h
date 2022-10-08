#pragma once

#include <QImage>
#include <QWidget>

namespace Ui {
    class cv_image_form;
};

class cvImageForm : public QWidget {
public:
    cvImageForm( const QImage& image, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~cvImageForm();

private slots:
    void loadImageFromFile();
    void saveImageToDb();

signals:
    void saveImage(const QImage&, QString);

private:
    Ui::cv_image_form* _UI;
    QImage m_Image;
private:
    Q_OBJECT
};
