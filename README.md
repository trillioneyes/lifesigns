lifesigns
=========

*Lifesigns* is a Game of Life simulator and surveyor. Instead of giving it a particular starting configuration, you give it a set of dimensions. It will then search through the space of starting configurations fitting that dimension and record a configurable set of statistics about the histories of the worlds it sees.

### Planned Features
* Store each grid space in a single bit, so that comparatively large dimensions can be surveyed
* Detect cycles by adapting the well-known algorithm for linked lists: keep track of a "fast" state and a "slow" state, and evolve the fast state by two steps each time. If the states are ever the same, there is a cycle.
* Simulate many starting configurations at once, rotating through them in such a way that every state will eventually be examined for arbitrarily long even if there are configurations that never die.
* Save runs of the program so that they can be continued later (for example, if the program must be stopped after surveying 1000 out of millions of possible configurations, it will start back up from configuration 1001).
* Load user-provided logging callbacks from a dynamic library.
* Optionally run some computations on the GPU.
