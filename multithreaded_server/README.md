# Multithreaded Server

## Pros and Cons

Without threads, each connection waits for the one that arrived first to finish.
However, each of these threads requires memory and CPU time. At some point, more threads are not gonna help and start killing the performance

Upsides:
Each connection does not need to wait for the one that arrived first to finish

Downsides:
1. could end up with a lot of threads
2. creating treads takes time

## References

https://www.youtube.com/watch?v=Pg_4Jz8ZIH4
