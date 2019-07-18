# OpenMP-Task-Suite
This suite is intended to provide a collection of applications that allows you to test the latest features of the OpenMP tasking implementations described in the latest versions of the OpenMP specification. In the rest of this README file are reported the minimal informations required to compile and run the relative application and a short description.

## Hashtag-Text Back-End Server
This program provides the implementation of a back-end server in charge of answering to simulated users requests for all those Texts held by the server itself and that are related to a particular Hashtag passed as input for the request. The server handles Hashtag-Text pairs as node items, each one connected into one of the several lists it actually maintains. In its turn, each list is associated to a particular bucket number and belongs to one of the several hashtables of which the server is equipped with. Further, each Text can be related to more than one Hashtag and the associated node may need to be connected to more than one list.

![Hashtag-Text-Data-Model](Images/Hashtag-Text-Architecture.png)

Since data is not replicated across the different hashtables, a request for all those Texts that are related to a given Hashtag mandatory needs to be propagated to all the hashtables before the server would be actually able to send a complete response to a user's request.
Despite the application can be compiled for sequential execution, we injected the proper OpenMP task directives into the code in order to exploit hardware parallelism offered by the target machine. More in detail, each simulated user request matches an OpenMP task which in turn generates as many tasks as there are hashtables, then it synchronizes with these tasks through a Taskwait directive since its output depends on the outputs of the latters. Moreover, each thread belonging to the team built on top of the parallel region can process any user request currently making available for the team.
