@echo off

mkdir ga4-win64
cd ga4-win64
cmake -G "Visual Studio 15 2017 Win64" ../../src/engine
cd ..
