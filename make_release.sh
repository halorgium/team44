#!/bin/sh

# $Header: /Users/tim/tmp/keep/uni_2004/to_keep/team44cvs/casestudy/make_release.sh,v 1.1 2003/09/25 14:30:35 tsm20 Exp $

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

cp $MODULE/Makefile $CLEANDIR || exit
cp -r $MODULE/src  $CLEANDIR || exit
mkdir $CLEANDIR/help
cp $MODULE/help/Makefile $CLEANDIR/help || exit
cp $MODULE/help/*.tex $CLEANDIR/help || exit
cp $MODULE/help/*.pdf $CLEANDIR/help || exit
cp -r $MODULE/help/usr_help $CLEANDIR/help || exit
cp -r $MODULE/var $CLEANDIR || exit

rm -f $CLEANDIR.tar.gz

tar zcvf $CLEANDIR.tar.gz $CLEANDIR
