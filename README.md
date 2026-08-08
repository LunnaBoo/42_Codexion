_This project has been created as part of the 42 curriculum by luccribe_

# Description
This project is a revamp of the old Philosophers project from 42, and very well known programming problem. Besides having it's own particularities
it tackles the same problems the original Philosophers is about: Multi-threading and concurrency. \
It is a great introduction to multi-threaded program and forces you to understand exactly how to manage multiple threads, or workers if the term
makes it more intuitive, using shared resources in the same program.

Which is essential knowledge to any developer theses days and C is the perfect language to teach concepts like this to us since it's very low level.
The less abstractions the language has the more you understand what is actually happening under the program's hood. \
And the project's subject also plays a role in making the project's development as bare bones as possible since it allows only a few external
functions and it prohibits standard library priority queues to force you to make the priority queue yourself from scratch.

All this combined makes this the perfect project to force a programmer to learn concurrency and multi-threading in depth, giving you solid knowledge
of these concepts instead of giving you shortcuts right off the bat. First learn it the hard way, then learn the easy way for day-to-day use.
# Instructions
Running the project is pretty simple, just run `make` for setup and you'll be set to actually run the program with `./codexion` + arguments. \
Here's the list of arguments the program needs in order to work, respectively:
1. number of coders
2. time to burnout
3. time to compile
4. time to debug
5. time to refactor
6. number of compiles required
7. dongle cooldown
8. scheduler

Example: \
`./codexion 5 100 200 200 200 3 50 fifo`

Explaining each argument:
1. Number of coders is self explanatory. It represents the number of coders in the simulation and the number of dongles too (dongles are like the forks
2. in original Philosophers).
3. Time to burnout means the time each coder can spend without compiling before burning out and ending the simulation.
4. Time to compile means the time each coder spends to compile.
5. Time to debug means the time each coder spends to debug.
6. Time to refactor means the time each coder spends to refactor.
7. Number of compiles required means how many times each coder has to compile to finish the simulation without burnouts.
8. Dongle cooldown is about the time a dongle stays inaccessible after it was used.
9. Scheduler refers to the arbitration policy used by dongles to decide who gets them when multiple coders request them. FIFO means First in First Out,
the most common queue known to people, and EDF means Earliest Deadline First.

# Blocking cases handled
I documented all of these in the wiki, so if you're interested in more details on this the wiki will be your friend. \
First obvious case was with the dongles. It is extremely easy to mess up the dongle requisition per coder and fall into a deadlock. \
Imagine this situation:
Coder	Dongles	Pedidos em ordem
```
Coder	Dongles	Requests, in order
1	    {0, 1}	0 → 1
2	    {1, 2}	1 → 2
3	    {2, 3}	2 → 3
4	    {3, 0}	3 → 0
```
What happens when Coder 4 holds dongle 3 and blocks onto dongle 0? Coder 1 holds dongle 0, but it will only release it once it gets
dongle 1 which is with Coder 2. Problem is Coder 2 is waiting for Dongle 2 which is with Coder 3 who is also waiting for a dongle, dongle 3, which is with Coder 4. \
Since Coder 4 will only release the dongle 3 after it gets dongle 0, every coder is now stuck waiting for a dongle that will never
arrive. Before I get into the solution to this in my implementation, I should talk about Coffman's conditions. \
These are a set of four conditions that need to happen simultaneously for a deadlock to (possibly) happen, they are:
1. Mutual exclusion: No two processes can obtain a resource at the same time.
2. Hold-and-wait: Once a resource is obtained, a process keeps the resource locked.
3. Circular wait: A cycle where each thread waits on a resource held by the next (dongle example above).
4. No pre-emption: Resources cannot be forcibly removed from a task.

Based on this, my solution aimed to fix the circular wait ordering dongle requests in ascending dongle-id order. So instead of Coder 4 holding dongle 3 and waiting for dongle 0, it requests
dongle 0 first and holds nothing while it waits for it to be granted. This breaks the cycle. \
Updated scenario:
```
Coder	Dongles	Requests, in order
1	    {0, 1}	0 → 1
2	    {1, 2}	1 → 2
3	    {2, 3}	2 → 3
4	    {3, 0}	0 → 3
```
Deadlock fixed.

There was also the single coder edge-case which resulted in a coder requesting the same dongle twice, but this was fixed in one line so there isn't much to talk about.

# Thread Synchronization mechanisms
This is also documented in the wiki, but I think I can get into further detail here.
I'll explain the threading primitives used to sync all threads and make this multi-threaded program work:
##### pthread_t
It's the data type used to uniquely identify a thread. In our code it is the thread, in simple terms.
##### pthread_mutex
(section translated directly from the repository's wiki)
The solution to race conditions. It is like a lock that protects a block of code and can only be executed by it's owner until they unlock it themselves.
With this a thread can lock, modify the variable locked, and only then unlock. If a second thread tries to modify the variable at the same time it will fail because of the mutex lock applied
by the first one. \
Also important to note that it is crucial to initialize and then destroy the mutex after it's done it's job. It doesn't work otherwise.
##### pthread_cond_t
It's a condition variable, the pthread type for "sleep until signaled" bell. Besides my description of it, a condition variable has no condition inside, it just meant to send a signal. The condition itself is just data protected by a mutex. \
This has to be paired to a mutex to avoid race conditions (two or more threads changing the shared data) and avoid lost wake-up. The latter happens when a thread checks the condition == false, then the condition is changed (no mutex preventing it) and the thread sleeps anyway since it's checked evaluated to false.
Because of this, in order to use pthread_cond_t it is essential to lock and unlock the data being used as condition. This primitive is crucial to thread syncing and communication, it'd be impossible to develop this program without it.
# Resources
Besides being a philosophers guide, it still helped me to sketch my plan for each step of the project's code. Helped me decide the structs I'd use
and the project's core logic. \
https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

Introduction to threads, also pretty useful. I even took some screenshots to use in the Wiki of this repo. \
https://www.youtube.com/watch?v=LOfGJcVnvAk

Rob Pike, co-creator of Go, talk about concurrency. I've actually stumbled upon this before even
starting Codexion and thanks to it I had good context on what this project needed from me. \
https://go.dev/blog/waza-talk

AI was used in this project's development through OpenCode Go to help with C syntax and write tons of tests to each functionality of this project. Tests weren't committed to this repository but were fundamental to avoid setbacks during development. \
It was useful to give wiki-related feedback, correct any misconceptions throughout both wiki and source code and to make development faster overall.
