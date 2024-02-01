#            SpringAlpha
An open-source lightweight antivirus engine

Info
This is an open-source lightweight, fast scanning virus engine that combines multiple scanning methods, 
including keyword scanning, PE import table scanning, signature scanning, and SVM scanning (under testing). 
This project is written purely in C++and does not rely on any cloud scanning

File description

KeyWordScan.cpp -> Search for virus keywords and pairing.

KeyWordScan.h -> Define virus keywords and head files.

PEFunctionScan.cpp -> Search for PE function keywords and pairing referenced by viruses.

PEFunctionScan.h -> Define the virus PE function and head files.

SignScan.cpp -> Check if the file signature is legal.

SignScan.h -> Define the head files.

SVMScan.cpp -> Scan files using support vector machines (Under testing)

SVMScan.h -> Define the head files.

head.h -> Define all virus scan head files.

main.cpp -> Main file.

Compilation environment

Use Visual Studio 2022's C++ Win32 Empty project to compile the project.

Run screenshot

![屏幕截图 2024-01-31 205753](https://github.com/JackyMark0w0/SpringAlpha/assets/158429850/5960a630-efa2-4c69-9416-6b19d2e9956b)

