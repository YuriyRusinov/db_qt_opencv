#include "dbImageClass.h"

dbImages::dbImages(long id, QString name, const QImage& im)
    : imageId(id),
    imageName(name),
    image(im) { }
