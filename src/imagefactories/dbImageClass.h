#pragma once

#include <QString>
#include <QImage>

class dbImages {
public:
    dbImages(long id=-1, QString name=QString(), const QImage& im=QImage());

    bool operator< (const dbImages& im1) const { return imageId < im1.imageId; }

    long getId() const { return imageId; }
    void setId( long id ) { imageId = id; }

    QString getName() const { return imageName; }
    void setName( const QString& name ) { imageName = name; }

    const QImage& getImage() const { return image; }
    void setImage( const QImage& im ) { image = im; }
private:
    long imageId;
    QString imageName;
    QImage image;
};
