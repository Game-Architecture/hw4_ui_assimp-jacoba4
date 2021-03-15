@echo off

mkdir ga4-win64
cd ga4-win64
cmake -G "Visual Studio 16 2019" ../../src/engine
cd ..
