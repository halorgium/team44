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
mkdir -p $INSTALL_DIR

# copy and chmod cgi files 
echo " Copying and chmoding cgi files" >&2
cp $CGI_FILES $INSTALL_DIR
chmod -R u+r $INSTALL_DIR
chmod -R o+rX $INSTALL_DIR

# make the data dir
echo " Making db dir" >&2
mkdir $DB_INSTALL_DIR

# copy and chmod db files
echo " Copying and chmoding db files" >&2
cp $DB_FILES $DB_INSTALL_DIR
chmod u+rw $DB_INSTALL_DIR/*
chmod og-rwx $DB_INSTALL_DIR/*
chmod og-rwx $DB_INSTALL_DIR

# chmod main dir
chmod o+rx $INSTALL_DIR

# print out pretty footer 
echo >&2
echo " ***************************** " >&2
echo " * Install successful        * " >&2
echo " ***************************** " >&2
echo >&2

