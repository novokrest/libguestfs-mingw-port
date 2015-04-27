#!/bin/bash

filename='.gitignore'

for f in `find . -name $filename`; do
    pdir=`dirname $f`
    oldfile=$pdir/$filename
    newfile=$pdir/not$filename
    mv $oldfile $newfile
done

