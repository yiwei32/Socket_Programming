# Multithreaded Server

## Server without threads

Without threads, each connection waits for the one that arrived first to finish.

## Server with threads
With threads, each connection does NOT need to wait for the one that arrived first to finish. Each of these threads, however, requires memory and CPU time. At some point, more threads are not gonna help and start killing the performance. Creating threads takes a little time. We can save some of that time by reusing the threads we create for future connection.


## Server with thread pool
Rather than creating a new thread for each connection that comes in, we have a set of threads we created at the begining. As works come in, we hand them to one of the threads. If there's no threads available, then that work just waits until there is a thread available. This allows us to do things at once and ensures that we don't create an unbounded number of threads.

However, the treads in thread pool are busily checking if there is more work, even when there's nothing to do. This is going to burn CPU cycles over and over again. To fix this, we need a condition variable.

## Condiction variable

let threads wait until something happens and it can do useful work.
waiting thread is going to wait until another thread call signal

## Reference

https://www.youtube.com/watch?v=Pg_4Jz8ZIH4
