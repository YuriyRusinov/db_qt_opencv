create table images (
    id serial not null primary key,
    name varchar not null,
    image_bytes bytea
);
