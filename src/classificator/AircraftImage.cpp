#include "AircraftImage.h"

AircraftImages::AircraftImages(long long id, QString name, const QImage& im)
    : imageId(id),
    imageName(name),
    image(im) { }
