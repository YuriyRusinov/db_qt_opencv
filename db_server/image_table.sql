create table aircraft_types (
    id bigserial not null primary key,
    id_parent bigint null,
    type_name varchar not null,
    type_description varchar,
    constraint fk_parent_type
        foreign key ( id_parent )
            references aircraft_types( id )
);    

create table aircraft_images (
    id bigserial not null primary key,
    id_type bigint,
    name varchar,
    image_bytes bytea,
    constraint fk_aircraft_type
        foreign key( id_type )
            references aircraft_types( id )
);
