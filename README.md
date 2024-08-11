# File to bytes utility
Usage for bytes view
> file2bytes.exe yourfile

Usage for save bytes to file from cmdline
> file2bytes.exe yourfile >> bytearray.c

Example ***.bat*** script for saving bytes to file. Place it next to the main program.
> @echo off<br>
> file2bytes.exe %1 >> %1_bytes.c