create or replace function image_insert(varchar, bytea) returns int4 as
$BODY$
declare
    idImage integer;
    imName alias for $1;
    imageBytes alias for $2;
begin
    select into idImage nextval('aircraft_images_id_seq'::regclass);
    insert into aircraft_images (id, name, image_bytes) values (idImage, imName, imageBytes);

    return idImage;
end
$BODY$
language 'plpgsql';

create or replace function set_image(int4, varchar, bytea) returns int4 as
$BODY$
declare
    idImage alias for $1;
    imageName alias for $2;
    imageBytes alias for $3;
begin
    update aircraft_images set name=imageName, image_bytes=imageBytes where id=idImage;
    return idImage;
end
$BODY$
language 'plpgsql';

create or replace function del_image(int4) returns int4 as
$BODY$
declare
    idImage alias for $1;
begin
    delete from aircraft_images where id=idImage;
    return idImage;
end
$BODY$
language 'plpgsql';
