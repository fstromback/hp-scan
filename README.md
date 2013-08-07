hp-scan
=======

Scanning utility for hp-scanners

This is a little "driver" for networked hp-scanners I wrote when I got sick of the original HP-driver.

This program will communicate with the printer over TCP/IP and read the scan to PNG, BMP or PDF files. Both flatbed and automatic document feeder is supported. It is only tested with HP MFP M1217, but might work with other HP-printers as well.

The program itself is built arount Boost asio for network support as well as libharu and libpng for png and pdf support.

It can be build using visual studio 2008 as well as gcc. A .mymake file is supported for building with mymake (fstromback/mymake). The command line arguments are fairly self-explanatory. They are shown when the executable is started without arguments.
