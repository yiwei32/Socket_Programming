# Socket_Programming

## Chatroom server (I/O multiplexing)

Use one thread to listen to multiple connetions.
I/O multiplexing is the capability to tell the kernel that we want to be notified if one or more I/O conditions are ready, like input is ready to be read, or descriptor is capable of taking more output.

Scenarios is which I/O multiplexing is used -

1. When client is handling multiple descriptors (like standard input and network socket).
2. When client handles multiple sockets at the same time, example - Web client.
3. When TCP server handles both listening and its connected sockets.
4. When server handles both TCP and UDP.
5. When server handles multiple services and perhaps multiple protocols.

## Demo


## References

https://youtu.be/_lQ-3S4fJ0U

https://youtu.be/bdIiTxtMaKA

https://youtu.be/dquxuXeZXgo

https://youtu.be/fNerEo6Lstw

https://github.com/lovenery/c-chatroom

https://www.thedailyprogrammer.com/2016/09/network-programming-io-multiplexing.html
