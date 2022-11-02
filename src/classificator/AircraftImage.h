#pragma once

#include <QString>
#include <QImage>

class AircraftImages {
public:
    AircraftImages(long long id=-1, QString name=QString(), const QImage& im=QImage());
    ~AircraftImages() {};

    bool operator< (const AircraftImages& im1) const { return imageId < im1.imageId; }

    long getId() const { return imageId; }
    void setId( long id ) { imageId = id; }

    QString getName() const { return imageName; }
    void setName( const QString& name ) { imageName = name; }

    const QImage& getImage() const { return image; }
    void setImage( const QImage& im ) { image = im; }
private:
    long long imageId;
    QString imageName;
    QImage image;
};
