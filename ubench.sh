#!/bin/bash

function usage {
	echo "Usage: ./ubench.sh <compile|clean|run>"
	echo "       compile - make all benchmarks"
	echo "       clean - clean all benchmarks"
	exit
}

function makeAll {	
	echo "==========================================="
	echo "Making binaries..."
	echo "==========================================="
	
	for bench in `ls ubench --format=single-column`
	do
		if [ -d ubench/$bench ]; then
			cd ubench/$bench
			make -f Makefile.small
			cd $OLDPWD
		fi
	done
	
	echo "==========================================="
	echo "Compilation finishes. Running simple tests"
	echo "==========================================="
	
	for bench in `ls ubench --format=single-column`
	do
		if [ -d ubench/$bench ]; then
			cd ubench/$bench
			make -f Makefile.small run-tests
			cd $OLDPWD
		fi
	done
}

function cleanAll {
	for bench in `ls ubench --format=single-column`
	do
		if [ -d ubench/$bench ]; then
			cd ubench/$bench
			make -f Makefile.small clean
			cd $OLDPWD
		fi
	done
}

if [ $# -eq 1 ]; then
	Mode=$1
else
	usage
fi

case $Mode in
	"compile" )
		makeAll ;;
	"clean" )
		cleanAll ;;
	* )
		usage ;;
esac
