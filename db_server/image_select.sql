create type image_res as (imageId bigint,
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
    query := E'select id, name, image_bytes from images';
    if(imgId is not null) then
        query := query || E' where id=' || imgId || E' order by 1;';
    else
        query := query || E' order by 1;';
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
