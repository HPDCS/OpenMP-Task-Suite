/**************************************************************************************************
 *                                                                                                *
 *  This program is part of the HPDCS OpenMP Task Suite.                                          *
 *                                                                                                *
 *  It provides the implementation of a back-end server in charge of handling simulated requests  *
 *  coming from some front-end users. Users just send queries to the system in order to retrieve  *
 *  all the texts (i.e. twitter texts) that are associated to a certain hashtag passed as input.  *
 *  One text may be associate to one or more hashtags, and the system relies on one or more hash- *
 *  tables to keep track of the hashtag-text pairs. Every hash-table has a predefined and static  *
 *  number of buckets, each one handling colliding pairs by mean of a double linked list. Since   *
 *  data is not replicated between hash-table instances, once a request arrives to the system a   *
 *  sub-query mandatory needs to be issued to any instance of hash-table.                         *
 *                                                                                                *
 *  We enriched the source code with OpenMP directives in order to exploit hardware parallelism.  *
 *  More precisely, we map each request to a first task who is in charge of generating as many    *
 *  tasks as there hash-tables. The latter are then in charge of retrieveng the texts associated  *
 *  to a given hashtag and returning them to the former task.                                     *
 *                                                                                                *
 **************************************************************************************************/


/**************************************
 * Set to 1 to print tasks completion *
 * value at runtime.                  *
 **************************************/
#define COMPLETED_REQUESTS_MONITORING		1


/****************************************
 * Number of Hashtable instances to use *
 * for storing Hashtag-Text pairs, and  *
 * number of buckets each Hashtable is  *
 * equipped of.                         *
 ****************************************/
#define STRINGS_HT_NUMBER					4
#define STRINGS_HT_BUCKETS					100


/***************************************
 * Max number of characters used to    *
 * maintain a sequence of Hashtags     *
 * associated to a given Text, and the *
 * Text itself.                        *
 ***************************************/
#define HASHTAG_MAX_LENGTH					200
#define TEXT_MAX_LENGTH						300


/***********************************
 * The Seed value is needed to run *
 * reproducible tests.             *
 ***********************************/
#define PSEUDO_RANDOM_SEED					1234


/*******************************************
 * This value is used by the Hash-function *
 * to associate a given Hashtag to a well  *
 * defined bucket number. Its purpose is   *
 * that of providing bucket's numbers that *
 * are uniformely distributed over the     *
 * admissible bucket range.                *
 *******************************************/
#define PRIME_NUMBER						103ULL


/******************************************
 * Determine how long the test will be in *
 * terms of maximum number of queries the *
 * system will deliver during execution,  *
 * and how many likely different Hashtags *
 * will be used to make a request.        *
 ******************************************/
#define REQUESTS_NUMBER						400000
#define MAX_DIFFERENT_REQUESTED_HASHTAGS	400000


/**********************************************
 * This value determines the maximum number   *
 * of results that can be returned from a     *
 * particular instance of Hashtable. Higher   *
 * this value and higher the memory allocated *
 * to accommodate the results retrieved from  *
 * each Hashtable instance.                   *
 **********************************************/
#define MAX_RESULTS_PER_HASHTAG				50


/************************************************
 * These three values define the probability    *
 * distribution of the three priority levels    *
 * (1, 2 and 3) associated to different classes *
 * of queries. While each one is admitted to    *
 * be a real number in the range [0.0, 1.0],    *
 * their sum must be always equal to 1.0.       *
 ************************************************/
#define PRIORITY_1_PROBABILITY				0.6
#define PRIORITY_2_PROBABILITY				0.3
#define PRIORITY_3_PROBABILITY				0.1


/*************************************************************
 * When NEW_NODES_INSERTION is defined at compile time, the  *
 * thread in charge of generating queries also produces      *
 * new Hashtag-Text pair insertion requests with probability *
 * equal to NEW_NODES_INSERTION. When this macro is enabled  *
 * all the accesses to a certain list of nodes are mutually  *
 * exclusive and threads synchronize among them through the  *
 * "omp_lock_t" spinlocks provided by the OpenMP spec.       *
 * One lock per bucket avoids race condition when multiple   *
 * threads attempt to perform lookups and insertions within  *
 * the same list.                                            *
 *************************************************************/
#ifdef NEW_NODES_INSERTION					/* e.g. 0.01 */
#define MAX_DIFFERENT_NEW_NODES				50000
#define NEW_NODES_INSERION_PROBABILITY		NEW_NODES_INSERTION
#endif


/***********************************************************
 * In case POISSON_ARRIVAL_TIME is defined at compile      *
 * time, this enables the thread, in charge of generating  *
 * queries, to follow a Poisson process distribution       *
 * with arrival time period equal to POISSON_ARRIVAL_TIME. *
 * This value must be expressed in seconds.                *
 ***********************************************************/
#ifdef POISSON_ARRIVAL_TIME					/* e.g. 0.000005 */
#define REQUEST_ARRIVALS_INTERVAL			POISSON_ARRIVAL_TIME
#define REQUEST_ARRIVALS_RATE				(1.0 / REQUEST_ARRIVALS_INTERVAL)
#endif