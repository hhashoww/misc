/* this file is also the input of Doxygen as the main page */

/**  @mainpage
    
This Library is developed by Genie Networks. The Library includiing 3 parts: Timer Tick, Timer List, and Timer Slot.
The Timer library will spawn a time ticking thread to execute the time-out tasks (callback funtion) defined by users. Users
must be very aware about the thread-safe or data synchronization (locking) issues while writing the call backs.

Users can use more than one timers if needed, just delacre the more variable of timer data structure and initialize it.
Every timer will spawn a new thread. The thread will be termintated will the users call the terminating functions.

####Using the library####
Users do not have to understand the detail data structure defined in this libray, just declare the variable and use the APIs
described in the following 3 chapters according to which kind of timer the user want to use. All the APIs are described in
the "Files and APIs" tab of the web page.

####Check out, Compile and Link####
Please visit the trac page for details under COMMON project (https://trac.genienrm.com/Common/wiki/GenieLibrary/GenieTimer).

####Sample codes and testing####
There are alpha testing tools under the "test" subdirectory when user check out the codes. The testing tools is also a 
sample code for users. Please see the "genie_timer_test.h" for how to execute different testing scenarios.

\<Where is the RCU and Timer Slot sample ?>

@internal code coverage testing using bullseye

\<How to test the code coverage using bullseye?>
@endinternal

- - - - - - - - - - - - - - - - - - - - - - - - - -

@section tick Section A: Timer Tick 
Timer Tick give the user a timer which will be ticked in a fixed time period. Users can define the tasks (call back 
function) to be done when the time is ticked. Basically, if users have a routine tasks whcih need to be done periodically
can utilize this library. 

### (A.1) Working Mechanisms of the Timer Tick: ###
(For Library Writers): The Timer Tick is using the same structure as Timer List, but use only one entry instead a list of 
entries. The Library will add a new entry to the list for every tick. Users do not need to handle the entry.
~~~~~~~~~~~~~~~~~~~~~
                                 Head / Tail
                                     |          
                                     V          
        +================+       +------------+   
        |   Timer  List  |  -->  | Tick Entry |     
        +================+       +------------+  
~~~~~~~~~~~~~~~~~~~~~
    The system structure is as above. 
         1. One Timer Tick only has ONE timeout delta time
         2. When user init a new Timer Tick, the library will add the Tick Entry automatically
         3. The Tick thread would do...  
            1. Sleep between each tick
            2. Reset the Tick Entry timeout value
            3. Call tick callback of the Tick Entry
            4. Back to item a.

    ### (A.2) Using the Timer Tick library: ###
    Programmers who want to use this library shall use the data structure and functios defined in "genie_timer.h"
    All the data types, data structures, constance defeine, and functions are in the "genie_timer.h" file. 
    Basicly, 1 data structure, and 2 functios below is necessary:
~~~~~~~
          Data Structure: 
               timer_tick_t        // for the timer tick handler;
          Functions:
               timer_tick_init();
               timer_tick_terminate();
~~~~~~~
    (A.2.1) The variable with type of timer_list_t  shall be declared before using it. 
        
               timer_tick_t     timer_tick;
                   
    (A.2.2) Write the initializtion funtion (if necessary) and timeout call back function before adding timer
                   
    (A.2.3) Use timer_tick_init() function to initialize the timer. For example:
 
               timer_tick_init(&timer_tick, 300, &init_f, &init_data, &timeout_f, &timeout_data);
               // init_f() is the function implemented by user, for the task to be done right before the timer tick begins;
               // init_data is the parameter of init_f();
               // timeout_f() is the function implemented by user, for the task to be done for each tick;
               // timeout_data is the paramte of timeout_f();
    (A.2.4) To terminate the timer, use timer_tick_terminate() function to terminate.

- - - - - - - - - - - - -

@section list Section B: Timer List
The Timer List is used for a sequential time event. Users can add timer entries to the list, but the timer will time out the
entries sequentially. It is suitable for user applications which need multiple timer event to be executed sequentially.
Because the timer is executed sequentially, the library only support "add timer entry" function. "delete" is not supported.
### (B.1) Working Mechanisms of Timer List: ###
~~~~~
                                Head                                            Tail 
                                 |                                               |
                                 V                                               V
      +================+       +-------+       +-------+       +-------+       +-------+
      |   Timer List   |  -->  | Entry |  -->  | Entry |  -->  | Entry |  -->  | Entry |   
      +================+       +-------+       +-------+       +-------+       +-------+
~~~~~
The system structure is as above. 
  1. One Timer List only has ONE timeout delta time
  2. New entry will be added after the Tail entry
  3. The List thread would do... 
        1. Sleep and wait for the HEAD entry timeout time is up
        2. Remove the HEAD entry (Next entry become HEAD)
        3. Call timeout callback of the removed entry
        4. Back to item a.

### (B.2) Using the Timer List library: ###
Programmers who want to use this library shall use the data structure and functios defined in "genie_timer.h"
All the data types, data structures, constance defeine, and functions are in the "genie_timer.h" file. 
Basicly, 2 data structure, and 3 functios below is necessary:
~~~~~~~~
      Data Structure: 
               timer_list_t        // for the timer list handler;
               timer_list_entry_t  // for a timer entry to be added;
      Functions:
               timer_list_init();
               timer_list_terminate();
               timer_list_add_entry();
~~~~~~~~
(B.2.1) The variable with type of timer_list_t  shall be declared as a handler of timer before using it. 
    
               timer_list_t     timer_list;
    
(B.2.2) Use timer_list_init() function to initialize the timer. For example:
    
               timer_list_init(&timer_list, 1000, &init_f, &init_data);
               // init_f() is the function implemented by user, for the task to be done right before the timer tick begins;
               // init_data is the parameter of init_f();


(B.2.3) Write the call back function, which will be executed when time is up, as the parameter timeout_f of timer_list_add_entry() function before adding timer

(B.2.4) To add a timer, you shall declare/allocate a timer entry with type of timer_list_entry_t, and the call the timer_list_add_entry() function to add the timer into the system with the callback functions and function argument.

The system will call the callback function while the time is up.

               timer_list_entry_t timer_list_entry;
               ......
               timer_list_add_entry(&timer_list, &timer_list_entry, &timeout_f, &timeout_argument);

(B.2.5) To terminate the timer, use timer_list_terminate() function to terminate.

- - - - - - - - - - - - - - -


@section slot Section C: Timer Slot
### (C.1) Working Mechanisms of Timer Slot: ### 
A Slot Timer is composed by serveral clocks. In our real world, we have SECOND, MINUTE, and 
HOUR. That is a case with 3 clocks. Different clocks have different tick granularities.
There are a number of slots in each clock. For example, "60 seconds count for 1 minutes" means
we need to have 60 slots in the "SECOND" clock, and 60 slots for the "MINUTE" clock.

~~~~~~~~~~
                                                   TIMER_SLOT
                                                   /  |  |  \
                           ------------------------   |  |   ---------------------- 
               CLOCK 0    /          CLOCK 1         /    \   CLOCK 2              \ CLOCK 3
        +--+--+--+---------+    +--+--+--+---------+    +--+--+--+---------+   +--+--+--+---------+
        |  +C0+  +         +    |  +  +C1+         +    |  +C2+  +         +   |  +C3+  +         +  <--SLOT array of CLOCK
        +--+--+--+---------+    +--+--+--+---------+    +--+--+--+---------+   +--+--+--+---------+
             |
             V
            Entry List
~~~~~~~~~~

The system structure is as above. The Timer has 4 Clocks. Each Clock equipped with an array of Slots. 
Each Slot may link to a lins of Entres. 

The Clock 0 is ticking every SYSTEM_TICK time, and increment the "current 0" (C0) tick counter. the C0 will wrap 
arround and overflow to C1, C1 will overflow to C2, C2 will overflow to C3.

The previous clock of Clock 1 is Clock 0, and the next clock of Clock 1 is Clock 2.

When the Clock 1 been ticked (by the overflow of previous clock, Clock 0), the system will distribute the timer
entry in C1 Slot to the Slot array of Clock 0. the same mechanism apply to Clock 2, 3. When Clock 0 been ticked, the 
System will purge the entries from the Slot C0, and call the callback functions of the entries one by one.
    
You may initialize several timer, which will spawn one timer thread each. if you have a very heavy loading call 
back task to executed, consider to split the task to several timers.


### (C.2) Using the Timer Slot library: ###
Programmers who want to use this library shall use the data structure and functios defined in "genie_timer.h"
All the data types, data structures, constance defeine, and functions are in the "genie_timer.h" file. 
Basicly, two data structure, and 4 functios below is necessary:

~~~~~
          Data Structure: 
                 timer_slot_t        // for the timer handler;
                 timer_slot_entry_t  // for a timer entry to be added or deleted;
          Functions:
                 timer_slot_init(); OR  timer_slot_init_advance();
                 timer_slot_terminate();
                 timer_slot_add_entry();
                 timer_slot_delete_entry();
~~~~~
(C.2.1) The variable with type of timer_slot_t  shall be declared before using it.

               timer_slot_t  timer_slot;

(C.2.2) Use timer_slot_init() function to initialize the timer. For example:

               timer_slot_init(&timer_slot,1000,&init_f, &init_data);

This function will use timer_tick library to spawn a ticking thread and tick the slot timer system every tick_time
and call the callback functions in the ticking thread. The above initilize the timer to tick every second(1000 ms)
, and call the init_f functios with parameter init_data in the spawned thread before doing anything. The users
may use the init_f to call the rcu_register_thread() if he want to use rcu data structure operation.
The initail number of slots for the 4 clocks are {16,64,64,256}. These numebr are defined in genie_timer.h
"CLOCK_SLOT_x_EXP" (which are 4,6,6,8), such that the library supports 2^(4+6+6+8) = 16*64*64*256 = 16,777,216 ticks
by default. In this case a tick is ONE second (1000 ms), this number of ticks will stands for 194 days 4 hours 20 
minutes 16 seconds.

If you want to change the number of slots in each clock, use timer_slot_init_advance() and specify the number of 
slot in exponential (non-zero) via the last interger array. 

           uint8_t	clock_slot_exp[NUM_CLOCK];
           .....
           clock_slot_exp[0] = 4;
           clock_slot_exp[1] = 4;
           clock_slot_exp[2] = 1;
           clock_slot_exp[3] = 1;
           timer_slot_init_advance(&timer_slot_1,150,&init_f,NULL, clock_slot_exp);
You will need more memory if you have large number of slots in clocks. Furthurmore, since the system is using a
32 bits variable to calculate the ticks, the sum of the clock_slot_exp[x] shall be less than 32.

(C.2.3) Writing call back function before add/delete/modify timer


<__IMPORTANT__> Please refer to Section (C.5) to understand how to write your call back function togathre with
your data structure.


(C.2.4) To add a timer, you shall declare/allocate a timer entry with type of timer_slot_entry_t, and the call the 
timer_slot_add() function to add the timer into the system with the callback functions and function argument. 
The system will call the callback function while the time is up.

              timer_slot_entry_t timer_slot_entry;
              ......
              timer_slot_add_entry(&timer_slot, &timer_slot_entry, time_out_seconds, &cbf, &cbf_argument);

(C.2.5) To delete a timer, use timer_slot_delete_entry() to delete the timer.

Please remember to free or recycle the timer entry after you delete the timer with a SUCCESS return code.
If the delete returns FAIL, it means the timer entry is now time up, purged and calling the call back function.
The data entry may not be free or recycled under this case.


(C.2.6) To modify a timer, used timer_slot_delete_entry() and timer_slot_add_entry() togather.
Since the data structure is recommened to use rcu mechanism to update, a new timer entry shall be used after 
deleteing the old timer entry.

              ......
              timer_slot_delete_entry(&timer_slot, &timer_slot_entry_old);
              ......
              timer_slot_add_entry(&timer_slot, &timer_slot_entry_new, time_out_tick, &cbf, &cbf_argument);
              ......
              recycle the timer_slot_entry_old which is deleted. 

(C.2.7) To terminate the timer, use timer_slot_terminate() function to terminate.


###(C.3) Use the library with your data structure ###

We recommend that the data structure use this timer under rcu mechanism. if yo do not understand what rcu is, please
contact your Technical Leader.
If you have a data entry need to be added into your data structure (hashing table, link list, etc.), and need a timer
in the mean time, you are recommended to follow the instruction below:

####(C.3.1) Declare your data entry: ####

We suggenst to have a timer entry memory togather data entry, take hasing as an example:
~~~~~
             struct hasing_data_entry {
                hlist *prev, *next;               // hashing data pointer
                struct _hash_key key;             // hashing key
                struct _data_entry data_entry;    // other data element....
                boolean in_hash;                  // TRUE if the entry is in hash
                timer_slot_entry_t timer;         // suggest to have a timer entry memory togather with data.
                };
~~~~~
####(C.3.2) Add Data Entry: ####
~~~~~
              data_entry_add() {
                  get a new hashing data entry;
                  grcu_read_lock();
                  lock data node;
                  insert data to data structure 
                  add timer via timer_slot_add_entry()   //lock timer; unlock timer;
                  unlock data node;
                  grcu_read_unlock();
                  }
~~~~~
####(C.3.3) Modify timer and data_entry for data: ####

If you want to modify timer, or data_entry and timer togather, please following the rcu mechanism. That is, you
shall get an new data entry, delete the old entry, and then add the new entry into the data structure.
~~~~~
                  data_entry_and_timer_modify() {
                        get a new hashing data entry, new_data;
                        rcu_read_lock();
                        lock data node;
                        find the original_data entry; make sure it is in_hash
                        if (data not in_hash)  return FAIL;
                        copy the data from the original_data entry to new_data entry;
                        modify data element in the new_data entry;
                
                        delete the timer_slot via timer_slot_delete_entry() for the original_data entry;
                        //Note: please be aware of the following if condition to check the return code
   
                        add new timer via timer_slot_add_entry()  // for new_data;
                        replace new_data to data structure, origin_data is deleted from data structure; 
                        set original data in_hash = FALSE;
                        
                        unlock data node;
                        rcu_read_unlock;
 
                        if (the return code of timer_slot_delete() is SUCCESS)  {
                           recycle the original data entry (with timer entry);   //use call_rcu() or grcu_call(), or synchronize_rcu
                        } else {
                            // the timeup callback function is going to be executed, 
                            // this data entry may be (a) modified by timer thread (refer to (C.3.5.a) 
                            //                        (b) deleting by timer thread (refer to (C.3.5.b)
                            // Since the timer call back will operate on the original entry, we can just do our modify.
                            // The timer call back will see the original_data is not in_hash, and shall be recycled.
                            // Since we have locked the data node here, the call back cannot get the lock and will wait until 
                            // unlock. Thus, timer thread callback will see the original_data is alread not in_hash.
                        }
                        return SUCCESS;
                 }
~~~~~

####(C.3.4) Deletion ####
~~~~~
          data_entry_deletion {
                  rcu_read_lock();
                  lock data node;
                  find the original data key entry; make sure it is still in hash
                  if (data not in_hash)  return FAIL;
                  delete the timer_slot (via timer_slot_delete_entry()) for the original data entry;
                  delete the data entry from data structure list (hasing list);
                  data in_hash = FALSE;
                  unlock data node;
                  grcu_read_unlock();

                  if (the return calue of timer_slot_delete_entry() above is SUCCESS) {
                     recycle the original data entry (with timer entry);   //use call_rcu() or grcu_call(), or synchronize_rcu
                  } else {
                     // the timeup callback function is going to be executed, 
                     // this data entry may be (a) modified by timer thread (refer to (C.3.5.a) 
                     //                        (b) deleting by timer thread (refer to (C.3.5.b)
                     // My suggestion:just delete the "data" but DO NOT recycle the data; 
                     // For case (C.5.a): the entry's call back function (running in timer thread) shall check the data 
                     //      is not in_hash, and recycle the data entry memory instead of modification.
                     // For case (C.5.b)the timer's call back function shall check the data is not in_hash, then recycle 
                     //      the memory to complete the deletion.
                  }
                  return SUCCESS;
            
            }
 ~~~~~

####(C.3.5) timeup callback function: ####
The timeup action maybe:

(a) Modify call back function : keep the data, but do some modify, then add new timer;<br>
(b) Purging call back function: delete the data, and free (recycle) the data and timer entry.
         
=====>The basic of writing these call back functions is to check the data in_hash, if not in_hash, then 
just recycle the data memory.
         
#####(C.3.5.a) Modify call back function: #####

The call back function's argument is a pointer pointing to the data, which will be modified. The call
back function's logic is like:
~~~~~ 
                      modify_call_back(*origin_data) {
                          lock the origin_data node.
                          check the data in_hash or not:
                          if (in_hash) {
                             get a new data memory;
                             add timer with the new data;
                             update/replace the data using new data.
                             set origin_data in_hash = FALSE;
                             }
                          unlock the data node;
                          recycle the origin_data and timer. // Might be deleted by other thread, C.4 "else" case
                       }
~~~~~
#####(C.3.5.b) Purgin call back function #####

The call back function's argument is a pointer pointing to the data, which will be purged. The call
back function's logic is like:
~~~~~                   
                     purge_call_back(*data) {
                          lock the data node.
                          check the data in_hash or not:     
                          if (in_hash) {
                             delete the data from data structure
                             set data in_hash = FALSE;
                             }
                          unlock the data node.
                          recycle the data and timer. // Might be deleted by other thread, C.4 "else" case
                       }
~~~~~

@file
Genie Timer Library (Including Timer Tick, Timer List, and Timer Slot). Please read the main page before using the library.
*/

/***********************************************************************************
* Copyright 2012 Genie Networks, All Rights Reserved
*
* Filename    : genie_timer.h
* Description : Genie Timer Library (Including Timer List, Timer Tick , and Timer Slot)
* Date Created: 2013-01-07
* Authors     : Richard Liu, Phil Peng
* Reviewd by  : 
* Edit History: 2014/1/6 add Doxygen Document format by Richard Liu.
***********************************************************************************/


#ifndef GENIE_TIMER_H
#define GENIE_TIMER_H

#include <stdint.h>
#include <sys/syscall.h>

// for White Bix Test
#ifdef _TIMER_WBTEST
#include "genie_timer_test.h"
#endif

#define GT_SUCCESS    1
#define GT_FAIL       0
#define GT_TRUE       1
#define GT_FALSE      0
#define CACHE_LINE   64

#ifndef likely
#define likely(x)   __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                 TIMER LIST DATA STRUCTURE                    ****************/
/*****************                                                              ****************/
/***********************************************************************************************/
// Timer List : Support add timer only, only one delta time value, assigne when init
// To use this library, you should
// (1) Alocate or declare memory for timer_list;
// (2) call timer_list_init(), this function will spawn a timer thread. The
//     thread will call a call-back function, which is specified by you, when
//     time is up.
// (3) write the call-back function. You may issue a new timer in the call-back.
//     you might need to free the timer_list_entry memory iu the call-back
//     if necessary.
// (4) Issue a timer via calling timer_list_add() and specify the call-back
//     function. Remember to allocat/declare the memory for timer_list_entry 

typedef enum {
    TIMER_LIST_STATE_WAIT_START  = 0,
    TIMER_LIST_STATE_START       = 1,
    TIMER_LIST_STATE_WAIT_EXIT   = 2,
    TIMER_LIST_STATE_EXIT        = 3
} timer_list_state_e;

struct timer_list_entry_s {    
    struct     timer_list_entry_s *next;          // The pointer to next entry. used by library system, users must not use it.
    uint64_t   timeout_msec;                      // Tne timeout time from the time adding the entry.
                                                  //    In the Timer Tick, this is the delta time between each tick. In the Timer List,
                                                  //    this is the delta time from the time adding this entry to the list.
    void       (*timeout_f)(void *userdata);      // The timeout call back functions implemented by users. executed when time is up.
    void       *userdata;                         // The parameter if the timeout_f() function.
};


//! The type of Timer List handler.
typedef struct {
    struct timer_list_entry_s   *head, *tail, *tick;
    uint64_t                    delta_msec;      //timeout time period of msec
    pthread_mutex_t             list_lock;
    pthread_t                   pthread;
    timer_list_state_e          timer_list_state;
    int                         graceful_tmnt;
    void                        (*init_f)(void *init_data);
    void                        *init_data;
    void                        (*tmnt_f)(void *tmnt_data);
    void                        *tmnt_data;
} timer_list_t;

typedef struct timer_list_entry_s  timer_list_entry_t;   //!< The data entry used for Timer List.
typedef struct timer_list_entry_s  timer_tick_entry_t;   //!< The data entry used for Timer Tick.
typedef timer_list_t               timer_tick_t;         //!< Defines the Timer Tick handler; use the same structure as Timer List handler.




/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                 TIMER SLOT DATA STRUCTURE                    ****************/
/*****************                                                              ****************/
/***********************************************************************************************/
// A Slot timer is composed by serveral clocks. In our real world, we have SECOND, MINUTE, and 
// HOUR. That is a case with 3 clocks. Different clocks have different tick granularities.
// There are a number of slots in each clock. For example, "60 seconds count for 1 minutes" means
// we need to have 60 slots in the "SECOND" clock, and 60 slots for the "MINUTE" clock.

#define NUM_CLOCK            4  //!< Number of clocks in the slot timer system
                                //   If you chnage the number of clocks, you shall also change CLOCK_x_SLOT_EXP
                                //   and related constance definition in timer_slot.c 

// The definition below will have the minimum time granularity for (tick_time).
// That is, if TICK_TIME is 1 second, the maximum timeout time is 
// 16*64*64*256 = 16,777,216 seconds, which is 194 day 4 hr 20 min 16 sec
#define CLOCK_0_SLOT_EXP     4  //!< The number of slots default value in the 1st clock is 2^4 = 16 
#define CLOCK_1_SLOT_EXP     6  //!< The number of slots default value in the 2nd clock is 2^6 = 64 
#define CLOCK_2_SLOT_EXP     6  //!< The number of slots default value in the 3rd clock is 2^6 = 64 
#define CLOCK_3_SLOT_EXP     8  //!< The number of slots default value in the 4th clock is 2^8 = 256

typedef uint32_t     tick_32bits;   //!< So, the sum of CLOCK_x_SLOT_EXP (TOTAL_SLOT_EXP) shall be less than 32

//TODO : change the list pointer to the list structure defined in list.h???

typedef struct timer_slot_entry_s {
    struct timer_slot_entry_s  *next, *prev;              // double link list pointer
    struct _slot         *slot;                            // pointer to the slot which this entry belongs to
    //uint16_t           rc;                               // reference count, TODO: Do we still need the reference count any more? I think not!!!
    tick_32bits          timeup_tick;                      // the tick value when this entry will be time up. In the slot head, this is the slot index.
    void                 (*callback_f)(void *userdata);    // call back function while time is up
    void                 *userdata;                        // argument of the call back function
} timer_slot_entry_t;   //!< Defines the data type of a entry in a timer slot

typedef struct _slot {         // the slot structure type
    timer_slot_entry_t   head;
    pthread_mutex_t      slot_lock;
    char                 pad[0] __attribute__ ((aligned (CACHE_LINE)));  //pad to align to cache line
} timer_slot_slot_t;

typedef struct _clock {         // the clock structure type  
    uint16_t             slot_exp;                         // the number of slots, in exponential
    uint16_t             tick_exp;                         // the number of timer system ticks to tick this clock, in exponential
    tick_32bits          tick_mask;                        // the mask to mask the tick bits belongs to this clock
    timer_slot_slot_t    *slot;                            // slot array pointer
    char                 pad[0] __attribute__ ((aligned (CACHE_LINE)));  //pad to align to cache line
} timer_slot_clock_t;

/*!
 A slot timer is composed by serveral clocks. In our real world, we have SECOND, MINUTE, and 
 HOUR. That is a case with 3 clocks. Different clocks have different tick granularities.
 There are a number of slots in each clock. For example, "60 seconds count for 1 minutes" means
 we need to have 60 slots in the "SECOND" clock, and 60 slots for the "MINUTE" clock.
*/
//! Defnies the type of Timer Slot handler.
typedef struct  {
    tick_32bits          tick_mask;                        // The mask for all the tick bits in the timer
    tick_32bits          ticked;                           // the number of ticked. for the power of 2 ticking method, support up to 32 bit to tick a round.
    uint32_t             tick_time;                        // The time for a system tick, unit by ms, value 1000 means 1 second
    uint64_t             start_msec;                       // The sysuptime for the timer begin to tick; msec
    timer_tick_t         tick_timer;                       // The tick timer, including the thread definition
    char                 pad[0] __attribute__ ((aligned (CACHE_LINE)));
    timer_slot_clock_t   clock[NUM_CLOCK];                 // a timer has several clock to tick in different granularity
} timer_slot_t;



/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                  UTILITY FUNCTION DEFINE                     ****************/
/*****************                                                              ****************/
/*****************         Return Value: milli-second since system reboot       ****************/
/*****************                       (0 Means Fail)                         ****************/
/*****************                                                              ****************/
/***********************************************************************************************/
/*!
This is a utility provided to users to get the sysuptime of the system. the absolute time of a system
sometimes will be changed via user configuration or time synchronization. However, the sysup time will not be affected.
@return the milli-second ellapsed since the system boot up.  
*/
uint64_t inline get_monotonic_msec(); // Milli-Second since system boot up



/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                 TIMER LIST FUNCTION DEFINE                   ****************/
/*****************                                                              ****************/
/*****************                Return Value: GT_SUCCESS or GT_FAIL           ****************/
/*****************                                                              ****************/
/***********************************************************************************************/
/*! 
The function used to initialize the Timer List and spawn the ticking thread for timer. After initialize the timer list, users can 
add a timer list entry into the timer list. The timeout time for each entry is fixed and is defined in this funtion.  
@param[in] timer_list The pointer of Timer List structure handler to be initialization. Users can use multiple ticking threads, 
                      but need multiple initialization on different Timer List stucture handler.
@param[in] delta_msec Timeout time for each entry in the list, unit in milli-second.
@param[in] init_f     The pointer of callback function before initializate Timer Tick, tick thread will first executed
                      this function and start AFTER this function is executed and returns.
                      Users should prepare this function if needed before calling the timer_tick_init() function. One sample usage of 
                      this init_f() fnction is to prepare user defined data and allocating memory required.
                      Use NULL function pointer if there is no initialization tasks to do.
@param[in] init_data  The parameter pointer for init_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.


@return GT_SUCCESS on success.GT_FAIL on failure.

@author Phil Peng <phil_peng@genie-networks.com>

<b>History: </b>First version in March 2013

<b>Bugs:</b> no known bugs

<b>See Also: </b>
@relatealso timer_list_add_entry(), timer_list_terminate()

*/
int timer_list_init(timer_list_t *timer_list,                              // pointer of timer list handler
                    uint64_t     delta_msec,                               // list timeout time, unit:milli-second
                    void         (*init_f)(void *init_data),               // init callback, list thread will Start AFTER this function return
                    void         *init_data);                              // the parameter for init_f

/*!
The function used to add a Timer List entry into the timer list handler specified in timer_list pointer. Please call timer_list_init() function to 
initialize the timer_list handler before adding any entry into it.
@param[in] timer_list The pointer of Timer List handler, specifying the list which the entry to be added to.
@param[in] new_entry  The pointer to a new timer entry to be added. The memory content cannot be reused until the entry is been timed out and removed
                      from the timer list. Users has to recycle the memory of the entry when time out. Usually, the memory management is imlemented in
                      the timeout_f() callback function of which pointer is specified as the parameter of this function.
@param[in] timeout_f  The pointer of the function which will be executed by the ticking thread when the time is up.
@param[in] user_data  The parameter pointer for timeout_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.


@return GT_SUCCESS on success.GT_FAIL on failure.

@author Phil Peng <phil_peng@genie-networks.com>

<b>History: </b>First version in March 2013

<b>Bugs:</b> no known bugs

<b>See Also: </b>
@relatealso timer_list_init(), timer_list_terminate()
*/
int timer_list_add_entry(timer_list_t       *timer_list,                   // pointer of timer list handler
                         timer_list_entry_t *new_entry,                    // pointer of the new timer list entry
                         void               (*timeout_f)(void *userdata),  // list timeout callback, list thread will Keep Working AFTER this function return
                         void               *userdata);                    // the parameter for timeout_f
/*!
The function used to terminate the Timer List ticking thread specified in the timer_list handler pointer.
@param[in] timer_list The pointer of Timer List handler.
@param[in] tmnt_f     The pointer of the function which will be executed by the tick thread right before the thread termination.
                      One sample usage of this function is to recyle/free memory which has been allocated while initializing the ticking thread by users.
                      Use NULL function pointer if there is no initialization tasks to do.
@param[in] tmnt_data  The parameter pointer for tmnt_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.
@param[in] graceful   Graceful termination of the ticking thread. Use GT_TRUE to indicate graceful terminaction, and the ticking thread will not be terminated
                      until the all the entries in the list is executed. Use GT_FALSE for non gracefull termination, the the ticking thread will terminate immediately.

@return GT_SUCCESS on success.GT_FAIL on failure.

@author Phil Peng <phil_peng@genie-networks.com>

<b>History: </b>First version in March 2013

<b>Bugs:</b> no known bugs

<b>See Also: </b>
@relatealso timer_list_init(), timer_list_add_entry()
*/

int timer_list_terminate(timer_list_t *timer_list,                         // pointer of timer list handler
                         void         (*tmnt_f)(void *tmnt_data),          // terminate callback, list thread will End AFTER this function return
                         void         *tmnt_data,                          // the parameter for tmnt_f
                         int          graceful);                           // graceful terminate the timer (0 means NO)

/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                 TIMER TICK FUNCTION DEFINE                   ****************/
/*****************                                                              ****************/
/*****************                Return Value: GT_SUCCESS or GT_FAI            ****************/
/*****************                                                              ****************/
/***********************************************************************************************/

/*! 
The function used to initialize the Timer Tick and spawn the tick thread.  
@param[in] timer_tick The pointer of Timer Tick handler to be initialization. Users can use multiple tick threads, 
                      but need multiple initialization on different Timer Tick Stucture handler.
@param[in] delta_msec Tick timeout time, unit in milli-second.
@param[in] init_f     The pointer of callback function before initializate Timer Tick, tick thread will first executed
                      this function and start AFTER this function is executed and returns.
                      Users should prepare this function if needed before calling the timer_tick_init() function. One sample usage of 
                      this init_f() fnction is to prepare user defined data and allocating memory required.
                      Use NULL function pointer if there is no initialization tasks to do.
@param[in] init_data  The parameter pointer for init_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.
@param[in] tick_f     The function pointer of tick timeout callback function. The tick thread will execute this function every tick.
                      Users should prepare this function if needed before calling the timer_tick_init() function.
@param[in] tick_data  The parameter pointer for tick_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.

@return GT_SUCCESS on success.GT_FAIL on failure.

@author Phil Peng <phil_peng@genie-networks.com>

<b>History: </b>First version in March 2013

<b>Bugs:</b> no known bugs

<b>See Also: </b>
@relatealso timer_tick_terminate()
*/
int timer_tick_init(timer_tick_t *timer_tick,                       // pointer of Timer Tick structure handler
                    uint64_t     delta_msec,                        // tick timeout time, unit:milli-second
                    void         (*init_f)(void *init_data),        // init callback, tick thread will Start AFTER this function return 
                    void         *init_data,                        // the parameter for init_f
                    void         (*tick_f)(void *tick_data),        // tick timeout callback, tick thread will Keep Working AFTER this function return
                    void         *tick_data);                       // the parameter for tick_f

/*!
The function used to terminate the Timer Tick tick thread specified in the timer_tick handler pointer.
@param[in] timer_tick The pointer of Timer Tick handler.
@param[in] tmnt_f     The pointer of the function which will be executed by the tick thread right before the thread termination.
                      One sample usage of this function is to recyle/free memory which has been allocated while initializing the tick thread.
                      Use NULL function pointer if there is no task to do.
@param[in] tmnt_data  The parameter pointer for tmnt_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                      an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting. 
@param[in] graceful   Graceful termination of the tick thread. Use GT_TRUE to indicate graceful terminaction, and the tick thread will not be terminated
                      until the next tick is executed. Use GT_FALSE for non gracefull termination, the the tick thread will terminate immediately.

@return GT_SUCCESS on success.GT_FAIL on failure.

@author Phil Peng <phil_peng@genie-networks.com>

<b>History: </b>First version in March 2013

<b>Bugs:</b> no known bugs

<b>See Also: </b>
@relatealso timer_tick_init()
*/

int timer_tick_terminate(timer_tick_t *timer_tick,                  // pointer of timer tick handler
                         void         (*tmnt_f)(void *tmnt_data),   // terminate callback, tick thread will End AFTER this function return
                         void         *tmnt_data,                   // the parameter for tmnt_f
                         int          graceful);                    // graceful terminate the timer (0 means NO)




/***********************************************************************************************/
/*****************                                                              ****************/
/*****************                 TIMER SLOT FUNCTION DEFINE                   ****************/
/*****************                                                              ****************/
/*****************                Return Value: GT_SUCCESS or GT_FAIL           ****************/
/*****************                                                              ****************/
/***********************************************************************************************/

/*!
This initialization function will initialize the timer, spawn a ticking thread to tick and execute callback functions.  It initialize all memory required
for the clock/slot structure.  We advice to use timer_slot together with grcu.  So please read the Section 3 of man page  to get the
sample pseudo code and know the coding desciplines.

@param[in] op_timer_slot A pointer of a timer slot handler. User shall declare the timer first and pass the pointer of the timer to the initialization functions.
@param[in] tick_time     The time for a tick, unit in milli-second. Setting the value as 1000 means 1 second.  This number decides the time granularity of a timer.  
                         The maximmum number of ticks supported by a timer is calculated by the numbers of slots for each clocks. Currently the library supports 
                         4 clocks, CLOCK 0 to CLOCK 3, which is define by the constant NUM_CLOCK.  The default numbers of slots power of 2 for each clocks are  
                         defined as [CLOCK_0_SLOT_EXP, CLOCK_1_SLOT_EXP, CLOCK_2_SLOT_EXP, CLOCK_3_SLOT_EXP], which are [4,6,6,8], such that the library supports
                         2^(4+6+6+8) = 16*64*64*256  = 16,777,216 ticks by default. If a tick is ONE second (1000 ms), 
                         this maximum number of ticks will stands for 194 days 4 hours 20 minutes 16 seconds.
                         User may change the numbers of slots for each clocks by assigning the clock_slot_exp[] array via using timer_slot_init_advance().
@param[in] init_f        The function which will be executed in the timer ticking thread right after the thread has been spawned and
                         before the first tick.User may use this mechanism to do initialization tasks for the timer slot ticking thread.  
                         For example, call rcu_register_thread() to prepare the timer thread co-working with the other rcu thread.  
                         Use NULL function pointer if there is no initialization tasks to do.
@param[in] init_data     The parameter pointer for init_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                         an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting. 

@return On success, return GT_SUCCESS. On error, GT_FAIL is returned.  The functions will return on error if the argument op_timer_slot is a NULL pointer.

@author Richard Liu <richard@genie-networks.com>
@author Phil Peng <phil_peng@genie-networks.com>

<b>See Also: </b>
@relatealso timer_slot_init_advance(), timer_slot_add_entry(), timer_slot_terminate(), timer_slot_delete_entry()
*/
int timer_slot_init(timer_slot_t  *op_timer_slot,                        // the pointer of the timer slot structure handler
                    uint32_t      tick_time,                             // time for a system tick, unit by ms, value 1000 means 1 second
                    void          (*init_f)(void *init_data),            // the init function will be called in the spawned thread
                    void          *init_data);                           // the parameter for init_f

/*!
This initialization function will initialize the timer, spawn a ticking thread to tick and execute callback functions.  It initialize all memory required
for the clock/slot structure.  We advice to use timer_slot together with grcu.  So please read the Section 3 of man page  to get the
sample pseudo code and know the coding desciplines.

@param[in] op_timer_slot A pointer of a timer slot handler. User shall declare the timer first and pass the pointer of the timer to the initialization functions.
@param[in] tick_time     The time for a tick, unit in milli-second. Setting the value as 1000 means 1 second.  This number decides the time granularity of a timer.  
                         The maximmum number of ticks supported by a timer is calculated by the numbers of slots for each clocks. Currently the library supports 
                         4 clocks, CLOCK 0 to CLOCK 3, which is define by the constant NUM_CLOCK.  The default numbers of slots power of 2 for each clocks are  
                         defined as [CLOCK_0_SLOT_EXP, CLOCK_1_SLOT_EXP, CLOCK_2_SLOT_EXP, CLOCK_3_SLOT_EXP], which are [4,6,6,8], such that the library supports
                         2^(4+6+6+8) = 16*64*64*256  = 16,777,216 ticks by default. If a tick is ONE second (1000 ms), 
                         this maximum number of ticks will stands for 194 days 4 hours 20 minutes 16 seconds.
                         User may change the numbers of slots for each clocks by assigning the clock_slot_exp[] array.
@param[in] init_f        The function which will be executed in the timer ticking thread right after the thread has been spawned and
                         before the first tick.User may use this mechanism to do initialization tasks for the timer slot ticking thread.  
                         For example, call rcu_register_thread() to prepare the timer thread co-working with the other rcu thread.  
                         Use NULL function pointer if there is no initialization tasks to do.
@param[in] init_data     The parameter pointer for init_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                         an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting. 

@param[in] clock_slot_exp  To set the numbers of slots for each clocks, power of 2.  Each number in this array is
                         between 1 to 255. Value 0 is forbidden. Using more slots in a clock requires more memory. Furthurmore, since the system is using a
                         32-bit variable (tick_32bits) to calculate the ticks, the sum of the clock_slot_exp[x] shall be less than 32.
                         See the description of argument tick_time above for details.

@return On success, return GT_SUCCESS. On error, GT_FAIL is returned.  The functions will return on error if the argument op_timer_slot is a NULL pointer.

@author Richard Liu <richard@genie-networks.com>
@author Phil Peng <phil_peng@genie-networks.com>

<b>See Also: </b>
@relatealso timer_slot_init_advance(), timer_slot_add_entry(), timer_slot_terminate(), timer_slot_delete_entry()
*/
int timer_slot_init_advance(timer_slot_t *op_timer_slot,                 // the pointer of the timer slot handler
                            uint32_t     tick_time,                      // time for a system tick, unit by ms, value 1000 means 1 second
                            void         (*init_f)(void *init_data),     // init callback, slot thread will Start AFTER this function return
                            void         *init_data,                     // the parameter for init_f
                            uint8_t      clock_slot_exp[NUM_CLOCK]);     // The exponential for number of slot of each clock
/*!
The function terminate and stop(kill) the specified timer_slot timer ticking thread. While calling this function, all the remained timer entries, of
which time is not up yet, will be given up.  The timer thread will stop running after executing the termination callback function tmnt_f().

@param[in] op_timer_slot  A pointer of a timer_slot_t structure pointing to the timer handler to be stopped.
@param[in] tmnt_f         The function which will be executed in the timer thread right before the thread stop running.   
                          User may use this mechanism to do termination tasks for the timer ticking thread. For example, call rcu_unregister_thread() 
                          to unregister the timer thread co-working with the other rcu thread.  
                          Use NULL function pointer if there is no termination task to do.
@param[in] tmnt_data      The parameter pointer for tmnt_f() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                         an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.

@return  On success, timer_slot_terminate() function returns GT_SUCCESS. On error, GT_FAIL is returned.  
         The functions will return on error if the argument op_timer_slot is a NULL pointer.

<b>Issues: </b><br>
The termination will not handle the remained timer entries such that these entries are difficult for User to handle them.
And I do not know if there is any other issues in the code.
 
@author Richard Liu <richard@genien-networks.com>

<b>See Also: </b>
@relatealso  timer_slot_init(), timer_slot_init_advance(), timer_slot_add_entry(), timer_slot_delete_entry()

*/
int timer_slot_terminate(timer_slot_t *op_timer_slot,                    // the pointer of the timer slot handler
                         void         (*tmnt_f)(void *tmnt_data),        // terminate callback, slot thread will End AFTER this function return
                         void         *tmnt_data);                       // the parameter for tmnt_f
/*!
The  timer_slot_add_entry()  function  add a timer entry into the timer.  The timer will use a spawned ticking thread to execute the callback function specified in the
argument after delta_tick.  We advice to use timer_slot together with grcu if you have complex data structure work with timer_slot.  So please read the Section 3 of man page
to get the sample pseudo codes and know the coding desciplines.

@param[in] op_timer_slot  A handler pointer of a timer slot handler. 
                          User shall declare the timer and initialize it via calling the timer_slot_init() function
                          before adding any timer entry into it.
@param[in] this_entry     The timer entry to be added. 
                          Users shall preapare the memory for the timer entry and then pass the pointer of the timer entry memory while
                          adding it into the timer_slot.
@param[in] delta_tick     The number of ticks to be timed out.
                          The actual time duration for a tick is specified by the timer_slot_init() function.
                          The value of delta_tick shall be > 0 and smaller than the maximum number of ticks.
                          The default maximum number of ticks is 2^24 = 16M and can be changed by calling the timer_slot_init_advance() function.
@param[in] cbf            The function and the function argument which will be executed in the timer thread right after delta_tick time.
@param[in] this_userdata  The parameter pointer for cbf() function. If users have data less than the size of a pointer (which is 64 bits in 64-bit system),
                          an immediate data can be used as a parameter instead of passing the pointer of data via proper type casting.

@return  On success, timer_slot_add_entry() function returns GT_SUCCESS. On error, GT_FAIL is returned.  
         The function will return on error if the argument delta_tick is invalid.

<b>ISSUES: </b>
       The function will not return FAIL when op_slot_timer and/or this_entry is a NULL pointer and may crash.

@author  Richard Liu <richard@genien-networks.com>

<b>See Also: </b>
@relatesalso       timer_slot_init(), timer_slot_init_advance(), timer_slot_terminate(), timer_slot_delete_entry(), timer_slot()
*/
int timer_slot_add_entry(timer_slot_t       *op_timer_slot,              // the slot timer handler to be operated
                         timer_slot_entry_t *this_entry,                 // the entry pointer to be added
                         uint32_t           delta_tick,                  // number of tick to timeout, shall be >0
                         void               (*cbf)(void *this_userdata), // will not change/assign the callback function if NULL
                         void               *this_userdata);             // argument pointer of callback_f
/*!
The timer_slot_delete_entry()  function  delete a timer entry from the timer slot.
We advice to use timer_slot together with grcu if you have complex data structure work with timer_slot.
So please read the Section 3 of man page to get the sample pseudo code and know the coding desciplines.

The deletion of a timer entry might be fail when the time is up for the timer entry, but the callback is not executed yet.

@param[in]  this_entry is the timer entry to be deleted.

@return
       On  success, timer_slot_delete_entry() function returns RT_SUCCESS. On fail, RT_FAIL is returned.  
       The function will return on fail if this_entry has been extract from the timer and waiting for the execution of callback function 
       specified by the timer entry while been added.

<b>Issues: </b>
       The function will not return GT_FAIL when this_entry is a NULL pointer and may crash while user assigening a inavlid pointer.

@author Richard Liu <richard@genien-networks.com>

<b>See Also: </b>
@relatesalso  timer_slot_init(), timer_slot_init_advance(), timer_slot_terminate(), timer_slot_add_entry(),
*/
int timer_slot_delete_entry(timer_slot_entry_t *this_entry);             // the pointer of the timer slot entry


#endif
