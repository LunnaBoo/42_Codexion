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

# Resources
Besides being a philosophers guide, it still helped me to sketch my plan for each step of the project's code. Helped me decide the structs I'd use
and the project's core logic. \
https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

Introduction to threads, also pretty useful. I even took some screenshots to use in the Wiki of this repo. \
https://www.youtube.com/watch?v=LOfGJcVnvAk
