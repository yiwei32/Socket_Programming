# Multithreaded Server

## Server without threads

Without threads, each connection waits for the one that arrived first to finish.

## Server with threads
With threads, each connection does NOT need to wait for the one that arrived first to finish. Each of these threads, however, requires memory and CPU time. At some point, more threads are not gonna help and start killing the performance. Creating threads takes a little time. We can save some of that time by reusing the threads we create for future connection.


## Server with thread pool
Rather than creating a new thread for each connection that comes in, we have a set of threads we created at the begining. As works come in, we hand them to one of the threads. If there's no threads available, then that work just waits until there is a thread available. This allows us to do things at once and ensures that we don't create an unbounded number of threads.

However, the treads in thread pool are busily checking if there is more work, even when there's nothing to do. This is going to burn CPU cycles over and over again. To fix this, we need a condition variable.

## Condiction variable

Condition Variable is a kind of Event used for signaling between two or more threads. One or more thread can wait on it to get signaled, while an another thread can signal this. Condition variable lets threads wait until something happens and it can do useful work. Threads are going to wait if there's no new work in queue.

How things actually work with condition variable:

* Thread 1 calls the wait on condition variable, which internally acquires the mutex and check if required condition is met or not.
* If not then it releases the lock and waits for Condition Variable to get signaled ( thread gets blocked). Condition Variable’s wait() function provides both these operations in atomic manner.
* Another Thread i.e. like Thread 2 signals the Condition Variable when condition is met
* Once Conditional Variable get signaled then Thread 1 which was waiting for it resumes. It then acquires the mutex lock again and checks if the condition associated with Condition Variable is actually met or if it is superiors call. If more than one thread was waiting then notify_one will unblock only one thread.
* If it was a superiors call then it again calls the wait() function. 

## Reference

1. https://www.youtube.com/watch?v=Pg_4Jz8ZIH4
2. https://thispointer.com/c11-multithreading-part-7-condition-variables-explained/
