#!/usr/bin/env bash
# @Author: mcxiaoke
# @Date:   2016-01-13 13:56:17
# @Last Modified by:   mcxiaoke
# @Last Modified time: 2016-01-13 14:35:21
find . -name Android.mk -path *src/main/jni* | while read line; do
    echo "------------------------------------------------------------"
    jpath=$(dirname $(dirname $line))
    echo "ndk-build NDK_DEBUG=1 $@ -C $jpath"
    ndk-build NDK_DEBUG=1 $@ -C $jpath
    ret=$?
    if [ $ret -ne 0 ] ; then
        echo "ndk-build command error, exit code: $ret"
        exit $ret
    fi
done
