create or replace function image_insert(varchar, bytea) returns bigint as
$BODY$
declare
    idImage bigint;
    imName alias for $1;
    imageBytes alias for $2;
begin
    select into idImage nextval('aircraft_images_id_seq'::regclass);
    insert into aircraft_images (id, name, image_bytes) values (idImage, imName, imageBytes);

    return idImage;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function set_image(bigint, varchar, bytea) returns bigint as
$BODY$
declare
    idImage alias for $1;
    imageName alias for $2;
    imageBytes alias for $3;
begin
    update aircraft_images set name=imageName, image_bytes=imageBytes where id=idImage;
    return idImage;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function del_image(bigint) returns bigint as
$BODY$
declare
    idImage alias for $1;
begin
    delete from aircraft_images where id=idImage;
    return idImage;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function set_image_type( bigint, bigint ) returns bigint as
$BODY$
declare
    idImage alias for $1;
    idType alias for $2;
begin
    update aircraft_images set id_type = idType where id=idImage;
    return idImage;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';
