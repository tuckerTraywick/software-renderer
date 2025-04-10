#!/bin/sh
echo "---- BUILDING ----"
./build.sh "$@"

if [ -f output/test ]; then
	echo "---- TESTING ----"
	output/test
	echo
fi

if [ -f output/run ]; then
	echo "---- RUNNING ----"
	output/run
fi
