#!/bin/sh

(cd src; indent -i4 -npsl -di0 -br -nce -d0 -cli0 -npcs -nfc1 -nut `find . |grep "\.c$" |grep -v "CVS" |grep -v "cgic"`)
