#!/usr/bin/env bash

set -e

mkdir -p coverage
gcovr --html --html-details coverage/coverage.html -r .
