#!/bin/sh

if [ "$WCC" == "" ]; then export WCC="x86_64-w64-mingw32-gcc"; fi
$WCC -I../HigherC/src-bkp -o bzsh src/*.c src/win/*.c
#$WCC -I../HigherC/src-bkp -o bzsh /usr/lib/libhigherc.a src/*.c src/win/*.c
#$WCC -I/usr/include/higherc -o bzsh /usr/lib/libhigherc.a src/*.c src/win/*.c
#$WCC -I/usr/include/higherc -o bzsh src/*.c src/win/*.c
#src/terminal/*.c 
#../higherc/src/*.c
