#pragma once

#include <QImage>
#include <QWidget>

namespace Ui {
    class cv_image_form;
};

class cvImageForm : public QWidget {
public:
    cvImageForm( qlonglong id=-1, const QString& imName=QString(), const QImage& image=QImage(), QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~cvImageForm();

private slots:
    void loadImageFromFile();
    void saveImageToDb();
    void classifyImage();

signals:
    void saveImage(const QImage&, QString, qlonglong);
    void classifyImage( const QImage&, QString, qlonglong );

private:
    Ui::cv_image_form* _UI;
    qlonglong m_id;
    QString m_ImageName;
    QImage m_Image;

private:
    Q_OBJECT
};
