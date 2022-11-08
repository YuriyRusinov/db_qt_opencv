#include "AircraftImage.h"

AircraftImage::AircraftImage(long long id, QString name, const QImage& im)
    : imageId(id),
    imageName(name),
    image(im),
    m_type( nullptr ) { }
