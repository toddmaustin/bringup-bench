#!/bin/bash

set -e

make TARGET=host all-clean
make TARGET=host run-tests

make TARGET=standalone all-clean
make TARGET=standalone run-tests

make TARGET=hashalone-host all-clean
make TARGET=hashalone-host run-tests

make TARGET=simple all-clean
make TARGET=simple run-tests

make TARGET=spike32 all-clean
make TARGET=spike32 run-tests

make TARGET=spike64 all-clean
make TARGET=spike64 run-tests

make TARGET=spike32-pk all-clean
make TARGET=spike32-pk run-tests

make TARGET=spike64-pk all-clean
make TARGET=spike64-pk run-tests

make TARGET=hashalone-spike32 all-clean
make TARGET=hashalone-spike32 run-tests

make TARGET=hashalone-spike64 all-clean
make TARGET=hashalone-spike64 run-tests

