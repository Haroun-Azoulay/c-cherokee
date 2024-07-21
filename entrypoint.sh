#!/bin/bash

make

if [ "$MODE" == "test" ]; then
  make test
else
  make run
fi