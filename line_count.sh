#!/bin/sh

(cd src; wc -l `find . |grep "\.[hc]$" |grep -v "CVS" |grep -v "cgic"`)
