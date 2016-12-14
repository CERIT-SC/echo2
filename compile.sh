#!/bin/bash

cd Echo

g++ -std=c++11 -pthread -O3 -m64 *.cpp

cd ..

mkdir Build

mv Echo/a.out Build/Echo
