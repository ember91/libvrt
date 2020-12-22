#!/usr/bin/env bash

mkdir -p coverage
gcovr --html --html-details coverage/coverage.html -r .
