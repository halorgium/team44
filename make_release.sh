#!/bin/sh

# $Header: /Users/tim/tmp/keep/uni_2004/to_keep/team44cvs/casestudy/make_release.sh,v 1.5 2003/09/29 09:36:48 djb142 Exp $

CLEANOUT="releases"
   CVSIN=`cat CVS/Root`
  MODULE="casestudy"

rm -fr $CLEANOUT || exit
mkdir $CLEANOUT || exit
cd $CLEANOUT || exit

echo "pwd="`pwd` || exit
rm -fr $MODULE || exit

cvs -d $CVSIN export -f -r HEAD $MODULE || exit

# copy needed files

(cd $MODULE/help; make)

CLEANDIR=musiclib-1.0

rm -fr $CLEANDIR || exit
mkdir $CLEANDIR || exit

cp $MODULE/do_install.sh $CLEANDIR || exit
cp $MODULE/main_vars.in $CLEANDIR || exit
cp $MODULE/Makefile $CLEANDIR || exit
cp -r $MODULE/src  $CLEANDIR || exit
mkdir $CLEANDIR/help
cp $MODULE/help/Makefile $CLEANDIR/help || exit
cp $MODULE/help/.htaccess $CLEANDIR/help || exit
cp $MODULE/help/*.tex $CLEANDIR/help || exit
cp $MODULE/help/*.pdf $CLEANDIR/help || exit
cp -r $MODULE/help/usr_help $CLEANDIR/help || exit
cp -r $MODULE/var $CLEANDIR || exit

rm -f $CLEANDIR.tar.gz

tar zcvf $CLEANDIR.tar.gz $CLEANDIR

rm -fr $CLEANDIR
rm -fr $MODULE

