#pragma once

#include <QString>
#include <QMetaType>
#include <vector>
#include <map>
#include <memory>

using std::map;
using std::vector;
using std::shared_ptr;

class AircraftType {
public:
    AircraftType( long long id=-1, const QString& name=QString(), const QString& desc=QString() );
    AircraftType( const AircraftType& aType );
    ~AircraftType();

    long long getId() const;
    void setId( long long id );

    QString getName() const;
    void setName( const QString& name );

    QString getDescription() const;
    void setDescription( const QString& desc );

    shared_ptr< AircraftType > getParent() const;
    void setParent( shared_ptr< AircraftType > pt );

    const map< long long, shared_ptr< AircraftType > >& childTypes() const;
    map< long long, shared_ptr< AircraftType > >& childTypes();
    void setChildTypes( const map< long long, shared_ptr< AircraftType > >& chTypes );

private:
    long long m_id;
    QString m_TypeName;
    QString m_TypeDescription;

    shared_ptr< AircraftType > m_parent;
    map< long long, shared_ptr< AircraftType > > m_childTypes;
};

Q_DECLARE_METATYPE( AircraftType );
Q_DECLARE_METATYPE( AircraftType* );
Q_DECLARE_METATYPE( shared_ptr< AircraftType > );
