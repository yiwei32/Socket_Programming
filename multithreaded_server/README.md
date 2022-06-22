# Multithreaded Server

## Server without threads

## Server with treads

## Server with tread pool
Rather than creating a new thread for each connection that comes in, we have a set of threads we created at the begining. As works come in, we hand them to one of the threads. If there's no threads available, then that work just waits until there is a thread available. This allows us to do things at once and ensures that we don't create an unbounded number of threads.

## Discussion

Without threads, each connection waits for the one that arrived first to finish.
However, each of these threads requires memory and CPU time. At some point, more threads are not gonna help and start killing the performance.
Creating threads takes a little time. We can save some of that time by reusing the threads we create for future connection.


## Reference

https://www.youtube.com/watch?v=Pg_4Jz8ZIH4
