#pragma once

#include <QString>
#include <QMetaType>
#include <QImage>
#include <memory>

using std::shared_ptr;

class AircraftType;

class AircraftImage {
public:
    AircraftImage(long long id=-1, QString name=QString(), const QImage& im=QImage());
    ~AircraftImage() {};

    bool operator< (const AircraftImage& im1) const { return imageId < im1.imageId; }

    long getId() const { return imageId; }
    void setId( long id ) { imageId = id; }

    QString getName() const { return imageName; }
    void setName( const QString& name ) { imageName = name; }

    const QImage& getImage() const { return image; }
    void setImage( const QImage& im ) { image = im; }

    shared_ptr< AircraftType > getType() const { return m_type; }
    void setType( shared_ptr< AircraftType > aType ) { m_type = aType; }
private:
    long long imageId;
    QString imageName;
    QImage image;

    shared_ptr< AircraftType > m_type;
};

Q_DECLARE_METATYPE(AircraftImage);
Q_DECLARE_METATYPE(shared_ptr<AircraftImage>);
