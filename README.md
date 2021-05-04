# Käyttöjärjestelmät ja systeemiohjelmointi: project 2
Unix utilities - a part of my course project for systems programming course </br></br>
This repository contains a few small programs. They are my implementations of some common UNIX utilities. 

## Compilation
Each program can be compiled with command "make [program name]" <br></br>
For example: make my-cat

## my-cat
my-cat reads a user-specified file or files and prints the contents. <br></br>
running: ./my-cat [filename(s)]

## my-grep
my-grep reads stdin or user-specified file(s) and prints all the lines that contain a search word. If the program is invoked with only one command line argument, it searches for the search word in stdin. With more command line arguments, the program searches for the search word in the specified files. <br></br>
running: ./my-grep [searchword] [filename(s) (optional)]

## my-zip and my-unzip
These programs are used for compressing and un-compressing files. The method used for file zipping is RLE (run-length encoding). <br></br>
my-zip.c and my-unzip.c use a shared library ziplib. <br></br>
running: ./my-zip [input file(s)] [> compressed file (optional)] <br></br>
./my-unzip [compressed file] [> output file(optional)] <br></br><br></br>
If output file is not specified, my-zip will print the output of the compression in stdin. my-unzip will print the output of the un-compression in stdin. Thus, generally the desired way to use the programs is by specifying an output zipped/unzipped file.


