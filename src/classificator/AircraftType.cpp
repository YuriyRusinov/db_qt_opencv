#include "AircraftType.h"

AircraftType::AircraftType( long long id, const QString& name, const QString& desc )
    : m_id( id ),
    m_TypeName( name ),
    m_TypeDescription( desc ),
    m_parent( nullptr ),
    m_childTypes( map< long long, shared_ptr< AircraftType > >() )
{
}

AircraftType::AircraftType( const AircraftType& aType )
    : m_id( aType.m_id ),
    m_TypeName( aType.m_TypeName ),
    m_TypeDescription( aType.m_TypeDescription ),
    m_parent( aType.m_parent ),
    m_childTypes( aType.m_childTypes )
{
}

AircraftType::~AircraftType() {}

long long AircraftType::getId() const {
    return m_id;
}

void AircraftType::setId( long long id ) {
    m_id = id;
}

QString AircraftType::getName() const {
    return m_TypeName;
}

void AircraftType::setName( const QString& name ) {
    m_TypeName = name;
}

QString AircraftType::getDescription() const {
    return m_TypeDescription;
}

void AircraftType::setDescription( const QString& desc ) {
    m_TypeDescription = desc;
}

shared_ptr< AircraftType > AircraftType::getParent() const {
    return m_parent;
}

void AircraftType::setParent( shared_ptr< AircraftType > pt ) {
    m_parent = pt;
}

const map< long long, shared_ptr< AircraftType > >& AircraftType::childTypes() const {
    return m_childTypes;
}

map< long long, shared_ptr< AircraftType > >& AircraftType::childTypes() {
    return m_childTypes;
}

void AircraftType::setChildTypes( const map< long long, shared_ptr< AircraftType > >& chTypes ) {
    m_childTypes = chTypes;
}
