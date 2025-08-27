#!/bin/bash

set -e

make TARGET=host all-clean
make TARGET=host run-tests

make TARGET=standalone all-clean
make TARGET=standalone run-tests

make TARGET=simple all-clean
make TARGET=simple run-tests

make TARGET=spike all-clean
make TARGET=spike run-tests

make TARGET=spike-pk all-clean
make TARGET=spike-pk run-tests

make TARGET=hashalone-host all-clean
make TARGET=hashalone-host run-tests

make TARGET=hashalone-spike all-clean
make TARGET=hashalone-spike run-tests

