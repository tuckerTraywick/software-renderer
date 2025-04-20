#!/bin/sh
compiler=gcc
flags="-Wall -Wextra -Wpedantic -std=gnu11 -lminifb -framework Cocoa"

run() {
	echo "$1"
	eval "$1"
}

for arg in "$@"; do
	case $arg in
		debug) 
		debug=true
		flags+=" -g3"
		;;
		release)
		release=true
		flags+=" -O2"
		;;
		test)
		test=true
		;;
	esac
done

if [ "$debug" = true -a "$release" = true ]; then
	echo "Can't build debug and release at the same time."
	echo
	exit 1
fi

rm -rf build output
mkdir -p build output

for file in source/*.c; do
	name=$(basename $file .c)
	run "$compiler $flags $file -Iinclude -Isource -c -o build/$name.o"
	echo
done

if [ -f source/main.c ]; then
	run "$compiler $flags build/*.o -o output/run"
	echo
	rm build/main.o
fi

if [ "$test" = true ]; then
	for file in tests/*.c; do
		name=$(basename $file .c)
		run "$compiler $flags $file -Iinclude -Isource -Itests -c -o build/$name.o"
		echo
	done
	run "$compiler $flags build/*.o -o output/test"
	echo
fi
