. ./user.config

#BASE=$1
#USER=$2
#DB_HOST=$3
#DB_PORT=$4
#PASSWD=$5
#ENCODING=$6

OLD_DB=`psql -U $USER -p $DB_PORT -h $DB_HOST -l | grep $BASE | head -1 | cut -d ' ' -f 2`
    if [ "$OLD_DB" = "$BASE" ]; then
        printf "\n\nDATABASE $BASE exist in current cluster! That will be dropped if you continue. Proceed? (y or n) [n] :";

	read key
	
 	if [ "$key" = "y" -o "$key" = "Y" ]; then
	    dropdb -p $DB_PORT -h $DB_HOST -U $USER $BASE
    else
	    printf "Installation process cancelled!\n "
	    exit 1
	fi;
fi

/usr/bin/createdb -h $DB_HOST -p $DB_PORT -E $ENCODING -U $USER $BASE

/usr/bin/psql -f image_table.sql --host=$DB_HOST --port=$DB_PORT --username=$USER $BASE
/usr/bin/psql -f image_select.sql --host=$DB_HOST --port=$DB_PORT --username=$USER $BASE
/usr/bin/psql -f insert_image.sql --host=$DB_HOST --port=$DB_PORT --username=$USER $BASE
/usr/bin/psql -f types.sql --host=$DB_HOST --port=$DB_PORT --username=$USER $BASE
