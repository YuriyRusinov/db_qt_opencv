drop type if exists image_res cascade;
create type image_res as (imageId bigint,
                          imageTypeId bigint,
                          imageTypeName varchar,
                          imageName varchar,
                          imageBytes bytea
                          );

create or replace function GetImages(bigint) returns setof image_res as
$BODY$
declare
    imgId alias for $1;
    r image_res%rowtype;
    query varchar;
begin
    query := E'select i.id, t.id, t.type_name, i.name, i.image_bytes from aircraft_images i left join aircraft_types t on( i.id_type = t.id ';
    if(imgId is not null) then
        query := query || E' and i.id=' || imgId || E') order by 1;';
    else
        query := query || E') order by 1;';
    end if;

    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
