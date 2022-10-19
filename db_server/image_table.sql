create table images (
    id bigserial not null primary key,
    name varchar,
    image_bytes bytea
);
