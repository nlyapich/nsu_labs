#!/bin/bash

lcov --capture --directory . --output-file bigInt.info --test-name bigIntTest
genhtml bigInt.info --output-directory output --title "JCI GoogleTest/LCOV BigInt" --show-details --legend
