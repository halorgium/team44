#!/bin/sh

# include vars
. main_vars 

# print out some pretty title
echo >&2
echo " ***************************** " >&2
echo " * Team 44 Music Library     * " >&2
echo " ***************************** " >&2
echo " * Authors:                  * " >&2
echo " * Tim Carey-Smith           * " >&2
echo " * Jeremy King               * " >&2
echo " * Eden Sinclair             * " >&2
echo " * Daniel Bakker             * " >&2
echo " ***************************** " >&2
echo >&2
echo " Installing to '"$INSTALL_DIR"'" >&2
echo -n " Continue [Y/n] ? " >&2
ans="y"
read ans
if [[ "z"$ans == "zN" ]]; then
	# set ans to n
	ans=n
fi
if [[ "z"$ans == "zn" ]]; then
	# exit from script 
	echo " Install aborted" >&2
	echo >&2
	exit 2
fi


# check whether install dir is present
if [[ -d $INSTALL_DIR ]]; then
	# is present
	echo " Install dir already exists; " >&2
	echo -n " Erase [y/N] ? " >&2
	ans="n"
	read ans
	if [[ "z"$ans == "zY" ]]; then
		# set ans to y
		ans=y
	fi
	if [[ "z"$ans == "zy" ]]; then
		# remove that directory
		rm -fr $INSTALL_DIR
	else
		# unable to proceed
		echo " Install impossible: directory in the way" >&2
		echo >&2
		exit 1
	fi
fi

echo

# make the cgi dir
echo " Making cgi dir" >&2
mkdir -p $INSTALL_DIR 2>&1 >/dev/null || (echo " error making cgi dir" >&2 && exit 4)

# copy and chmod cgi files 
echo " Copying and chmoding cgi files" >&2
cp $CGI_FILES $INSTALL_DIR 2>&1 >/dev/null || (echo " error copying cgi files: make before make install" >&2 && exit 3)
chmod -R o+rX $INSTALL_DIR 2>&1 >/dev/null 

# make the data dir
echo " Making db dir" >&2
mkdir -p $DB_INSTALL_DIR 2>&1 >/dev/null || (echo " error making db dir" >&2 && exit 4)

# copy and chmod db files
echo " Copying and chmoding db files" >&2
cp $DB_FILES $DB_INSTALL_DIR 2>&1 >/dev/null || (echo " error copying db files" >&2 && exit 3)
chmod u+rw $DB_INSTALL_DIR/* 2>&1 >/dev/null 
chmod -R og-rwx $DB_INSTALL_DIR 2>&1 >/dev/null 

TMP_DIR=$INSTALL_DIR 
RES=0
# loop towards / from $INSTALL_DIR
while [[ $RES -eq 0 ]]; do
	chmod o+x $TMP_DIR 2>/dev/null >/dev/null
	RES=$?
	TMP_DIR=$TMP_DIR"/.."
	if [[ $RES -eq 0 ]]; then
		echo " chmod on '"$TMP_DIR"'" >&2
	fi
done

# print out pretty footer 
echo >&2
echo " ***************************** " >&2
echo " * Install successful        * " >&2
echo " ***************************** " >&2
echo >&2

