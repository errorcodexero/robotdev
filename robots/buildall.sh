#!/bin/bash

ALLROBOTS="theremin claymore play"

for robot in $ALLROBOTS ;
do
    rm -rf common/builddir
    pushd $robot > /dev/null
    rm -rf builddir
    make CONFIG=debug
    if [ $? -ne 0 ]; then
	break
    fi
    
    make CONFIG=release
    if [ $? -ne 0 ]; then
	break
    fi
    
    popd > /dev/null
done

