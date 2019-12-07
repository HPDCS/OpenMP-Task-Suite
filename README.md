# OpenMP Task Suite
This suite is intended to provide a collection of applications that allow to test the latest features of the OpenMP tasking-model implementations (*e.g*, priority tasks) described only in the more recent versions of OpenMP specification. In the rest of this README file are reported the minimal informations required to compile and run the applications together with a short description of the internals.

## Hashtag-Text Service
This program provides the implementation of a back-end server in charge of answering to simulated users requests for all those Texts held by the server itself and that are related to a particular Hashtag passed as input for the request. The server handles Hashtag-Text pairs as node items, each one connected into one of the several lists it actually maintains. In its turn, each list is associated to a particular bucket number and belongs to one of the several hashtables of which the server is equipped with. Further, each Text can be related to more than one Hashtag and the associated node may need to be connected to more than one list.

<p align="center"><img src="https://raw.githubusercontent.com/HPDCS/OpenMP-Task-Suite/master/Images/Hashtag-Text-Architecture.png" alt="drawing" width="350"/></p>

Since data is not replicated across the different hashtables, a request for all those Texts that are related to a given Hashtag mandatory needs to be propagated to all the hashtables before the server would be actually able to send a complete response to a user's request.
Despite the application can be compiled for sequential execution, we injected the proper OpenMP task directives into the code in order to exploit hardware parallelism offered by the target machine. More in detail, each simulated user request matches an OpenMP task which in turn generates as many tasks as there are hashtables, then it synchronizes with these tasks through a Taskwait directive since its output depends on the outputs of the latters.

<p align="center"><img src="https://raw.githubusercontent.com/HPDCS/OpenMP-Task-Suite/master/Images/Hashtag-Text-Tasking-Model.png" alt="drawing" width="500"/></p>

Into the application folder you may find four files:

1. **hashtag-text-service.c** : contains the source code of the application including data structures and the initialization/finalization routines.

2. **hashtag-text-service.h** : this header file contains all the application's parameters set to the values of the default configuration. Each of these parameters can be changed to produce a different configuration:

    1. **STRINGS_HT_NUMBER** : indicates how many hashtable instances to use
    2. **STRINGS_HT_BUCKETS** : indicates how many buckets each hashtable is provided with
    3. **REQUESTS_NUMBER** : determines the total number of user requests arriving into the server
    4. **MAX_DIFFERENT_REQUESTED_HASHTAGS** : indicates how many different hashtag strings to retrieve at startup when generating the reservoir of requests that will be send at run-time
    5. **MAX_RESULTS_PER_HASHTAG** : determines the maximum number of texts that will be retrieved from each hash table
    6. **PRIORITY_X_PROBABILITY** : determines the probability value of having a request with priority X (3 priority levels are provided by default and the sum of their probability values must be always equal to 1.0)

3. **TextHashtag.txt** : this heavy file is read once in the initialization phase to populate data structures with Hashtag-Text pairs.

4. **Makefile** : used either to compile the application (with ***make***) or to clean object files (with ***make clean***). To run the application simply launch the executable (***./service***) from command line. Furthermore, the following flags are used to enable/disable and/or customize additional features that are described below:

    1. **POISSON_ARRIVAL_TIME** : when active provides an average time value (expressed in seconds) for a Poisson process distribution in order to simulate an exponential interarrival time between consecutive requests (disabled by default).
    
    2. **NEW_NODES_INSERTION** : when active enables some users requests (according to the given probability value) to be an insertion request, that is the associated task will perform the insertion of a new node into one list of one hashtable. To make it possible we rely on the *omp_lock_t* spinlocks, one per list, in order to ensure exclusive access to that list (disabled by default).

    3. **TIED_INSERT_REQUEST** : a value between [**tied**/**untied**] to indicate ...

    4. **TIED_INSERT_HASHTABLE** : a value between [**tied**/**untied**] to indicate ...

    5. **TIED_QUERY_REQUEST** : a value between [**tied**/**untied**] to indicate that the tasks representing the simulated users's requests, and associated to the function generating as many tasks as there are hashtables, must be tied or untied.

    6. **TIED_QUERY_HASHTABLE** : a value between [**tied**/**untied**] to indicate that the tasks performing lookup of a particular Hashtag into a single instance of hashtable must be tied or untied.

    7. **MANUAL_CUT_OFF** : when set to 1, no tasks are created in addition to the one associated to the request, resulting in a manually hand-coded cut-off strategy that prevents the application from injecting too much work into the system, *e.g.*, when the requests arrival rate is high (disabled by default).

Additional information can be read inside the header file, inline alongside with the source code.
