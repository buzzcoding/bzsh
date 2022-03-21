#!/bin/sh

if [ "$CC" == "" ]; then export CC="cc"; fi
$CC -o bzsh src/*.c src/unix/*.c
#$CC -I../HigherC/src-bkp -o bzsh /usr/lib/libhigherc.a src/*.c src/unix/*.c
#$CC -I/usr/include/higherc -o bzsh /usr/lib/libhigherc.a src/*.c src/unix/*.c
#$CC -I/usr/include/higherc -o bzsh src/*.c src/unix/*.c
#src/terminal/*.c 
#../higherc/src/*.c
