#pragma once

#include <QObject>

class OpenCVDatabase;
class dbLoader : public QObject {
public:
    explicit dbLoader( QObject* parent = nullptr );
    dbLoader( const dbLoader& dbl ) = delete;
    dbLoader& operator=( const dbLoader& dbl ) = delete;
    virtual ~dbLoader();

    friend class dbOpenCvS;
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
