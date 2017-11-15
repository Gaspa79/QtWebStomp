# QtWebStomp
A C++ implementation of the Stomp protocol through websocket

## How to build in Windows
Open visual studio, click build. As of now, only debug creates the build/bin and build/include structure. In release you have to create it yourself (it's just copy-pasting).

## How to use

Include the header in your source.
Just do an #include "QTWebStompClientDll.h" . Really, that’s all you need.
 
Create an instance of the client.
An example:
auto myClient = new QTWebStompClient("ws://10.12.4.142:15674/ws", "ugs", "ugs", onConnect, false);
Once the client is created, it will automagically connect. The constructor takes the URL of webstomp as first parameter, second is login, third is passcode. The fourth is a pointer to a function that will execute as soon as the client is connected aka the on connect callback (made it like the JS client that Andrew was working with for simplicity). You can set the last bool to true to see debug messages in the console.
 
Subscribe to a queue
On the onConnect method, you can subscribe to a Queue like this:
 
void onConnect() {
myClient->Subscribe("/queue/agent", onMessage);
}
 
The Subscribe method takes two mandatory parameters: the first one is the suburl you want to subscribe to (in this case the agent queue). The second one is a pointer to a function you want to execute when you receive a message (just like the JS client again). There’s a third optional parameter: the way you want to ack the messages (takes an enum). It defaults to auto (no manual ack). If you use the other options, you can ack messages with the ack method that the client contains. See example below.
 
Execute something when you receive a message
The onMessage function will be receiving a StompMessage object by constant reference. You can keep a copy of the object if you want, but then it’ll be your job to clean up afterwards =). If you don’t keep a copy there are no memleaks, I checked. (What is this, I have to manage my memory now?!? Welcome to C++!)
So an example of printing the message and acking it would be like this:
 
void onMessage(const StompMessage &s) {
              qDebug() << "The message we got is\r\n" << s.toString().c_str(); // The toString function shows the message in a very readable way.
              myClient->Ack(s); // You don’t need to ack the message if you use automode. You can either specify the entire message as a parameter of ack or just the id you wanna ack if you wanna ack it later but not save the entire object.
}
 
That’s it! You can access the individual headers with the s.m_headers vector and the message with s.m_message. 

## Known-bugs
None! (Yet)

## TODO
Add SSL

