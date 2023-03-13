#!/usr/bin/bash

./build.sh
if [ $? -eq 0 ]
then
./test_server.py
fg 1
fi
