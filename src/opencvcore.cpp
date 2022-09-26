#include <QtDebug>
#include "opencvcore.h"

bool OpenCVCore::GUIConnect(QWidget* parent, Qt::WindowFlags flags) {
    Q_UNUSED(parent);
    Q_UNUSED(flags);
    qDebug() << __PRETTY_FUNCTION__;
    return false;
}

OpenCVCore::OpenCVCore( QObject* parent )
    : QObject( parent ) {}

OpenCVCore::~OpenCVCore() {
}
