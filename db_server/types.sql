drop type if exists aircraft_type cascade;
create type aircraft_type as (id bigint,
                              id_parent bigint,
                              type_name varchar,
                              type_description varchar
                              );

create or replace function GetTypes(bigint) returns setof aircraft_type as
$BODY$
declare
    idParent alias for $1;

    query varchar;
    cnt integer;

    r aircraft_type%rowtype;
    rr aircraft_type%rowtype;
begin
    
    if( idParent is null ) then
        query := E'select id, id_parent, type_name, type_description from aircraft_types where id_parent is null order by 1';
    else
        query := E'select id, id_parent, type_name, type_description from aircraft_types where id_parent = ' || idParent || E' order by 1';
    end if;
    for r in
        execute query
    loop
        return next r;
        select into cnt count(*) from aircraft_types where id_parent = r.id;
        if (cnt != 0) then
            for rr in
                select * from GetTypes(r.id)
            loop
                return next rr;
            end loop;
        end if;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function GetType(bigint) returns setof aircraft_type as
$BODY$
declare
    id alias for $1;

    query varchar;
    cnt integer;

    r aircraft_type%rowtype;
    rr aircraft_type%rowtype;
begin
    
    query := E'select id, id_parent, type_name, type_description from aircraft_types where id=' || id || E' order by 1';
    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function insertType( bigint, varchar, varchar ) returns bigint as
$BODY$
declare
    idParent alias for $1;
    tName alias for $2;
    tDesc alias for $3;

    cnt integer;
    idType bigint;
begin
    if( idParent is not null ) then
        select into cnt count(*) from aircraft_types where id=idParent;
        if( cnt = 0 ) then
            raise warning 'Error in table aircraft_types, parent id is not found';
            return -1;
        elsif( cnt > 1 ) then
            raise warning 'Error in table aircraft_types, multiple ids';
            return -2;
        end if;
    end if;
    select into idType nextval('aircraft_types_id_seq'::regclass);
    insert into aircraft_types( id, id_parent, type_name, type_description ) values ( idType, idParent, tName, tDesc );
    return idType;

    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function updateType( bigint, bigint, varchar, varchar ) returns bigint as
$BODY$
declare
    idType alias for $1;
    idParent alias for $2;
    tName alias for $3;
    tDesc alias for $4;

    cnt integer;
begin

    select into cnt count(*) from aircraft_types where id = idType;
    if( cnt <> 1 ) then
        return -1;
    end if;
    update aircraft_types set id_parent=idParent, type_name = tName, type_description = tDesc where id = idType;
    return idType;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function delType( bigint ) returns bigint as
$BODY$
declare
    idType alias for $1;

    cnt integer;
begin
    select into cnt count(*) from aircraft_types where id = idType;
    if( cnt <> 1 ) then
        return -1;
    end if;
    delete from aircraft_types where id = idType;

    return idType;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';
