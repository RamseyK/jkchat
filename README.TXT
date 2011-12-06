jkchat

A simple chat client and server
http://code.google.com/p/jkchat/

AUTHORS
--------------------
Ramsey Kant - ramsey.kant@gmail.com
Keilan Jackson - krjackso@asu.edu


TESTED PLATFORMS
--------------------
* Mac OS X
* Debian 6.0
* Cygwin (server and command line client only)


BUILD INSTRUCTIONS
--------------------

make <target name>
ex. make server

targets:

server - The jkchat server is built and placed in the bin folder. No external dependancies

guiclient - Runs commands in the shell to build the guiclient. QtSDK (v474) must be installed. Executable placed in the guiclient folder. QtSDK assumed to be in default path (home folder) ~/QtSDK/

client - The command line client is built and placed in the bin folder. ncurses library & headers required
