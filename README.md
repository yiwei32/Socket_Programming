# Socket_Programming

## Chatroom server

Use one thread to listen to multiple connetions.
I/O multiplexing is the capability to tell the kernel that we want to be notified if one or more I/O conditions are ready, like input is ready to be read, or descriptor is capable of taking more output.

Scenarios is which I/O multiplexing is used -

When client is handling multiple descriptors (like standard input and network socket).
When client handles multiple sockets at the same time, example - Web client.
When TCP server handles both listening and its connected sockets.
When server handles both TCP and UDP.
When server handles multiple services and perhaps multiple protocols.

## References

https://youtu.be/_lQ-3S4fJ0U

https://youtu.be/bdIiTxtMaKA

https://youtu.be/dquxuXeZXgo

https://youtu.be/fNerEo6Lstw

https://github.com/lovenery/c-chatroom
