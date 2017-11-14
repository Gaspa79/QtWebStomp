# QtWebStomp
A C++ implementation of the Stomp protocol through websocket

## How to build in Windows
Open visual studio, click build. As of now, only debug creates the build/bin and build/include structure. In release you have to create it yourself (it's just copy-pasting).

## How to use
Add the include directory to your -I, and add #include "QTWebStompClientDll.h" to your sourcecode. Link the .lib and you're done! Just make sure you copy the .dll to your application directory or a system directory.
Check the test project on how to connect, set callbacks and ack messages (if needed).

## Known-bugs
None! (Yet)

## TODO
Add send-message feature

Add SSL

