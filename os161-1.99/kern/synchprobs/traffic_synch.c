#include <types.h>
#include <lib.h>
#include <synchprobs.h>
#include <synch.h>
#include <opt-A1.h>
#include <thread.h>

/* 
 * This simple default synchronization mechanism allows only vehicle at a time
 * into the intersection.   The intersectionSem is used as a a lock.
 * We use a semaphore rather than a lock so that this code will work even
 * before locks are implemented.
 */

/* 
 * Replace this default synchronization mechanism with your own (better) mechanism
 * needed for your solution.   Your mechanism may use any of the available synchronzation
 * primitives, e.g., semaphores, locks, condition variables.   You are also free to 
 * declare other global variables if your solution requires them.
 */

/*
 * replace this with declarations of any synchronization and other variables you need here
 */
//static struct semaphore *intersectionSem;
static struct lock *trafficLock = 0;
static int volatile traffic[12];



/* 
 * The simulation driver will call this function once before starting
 * the simulation
 *
 * You can use it to initialize synchronization and other variables.
 * 
 */
void
intersection_sync_init(void)
{
  /* replace this default implementation with your own implementation */
  for(int i=0;i<12;i++) {    
    traffic[i] = 0;
  }
  // intersectionSem = sem_create("intersectionSem",1);
  // if (intersectionSem == NULL) {
  //   panic("could not create intersection semaphore");
  // }
  if (trafficLock==NULL) {
    trafficLock = lock_create("trafficLock");
    if (trafficLock == NULL) {
      panic("traffic_synch.c: lock_create failed\n");
    }
  }
  return;
}

/* 
 * The simulation driver will call this function once after
 * the simulation has finished
 *
 * You can use it to clean up any synchronization and other variables.
 *
 */
void
intersection_sync_cleanup(void)
{
  /* replace this default implementation with your own implementation */
  KASSERT(trafficLock != NULL);
  //sem_destroy(intersectionSem);
  lock_destroy(trafficLock);
}


static void blockPaths(int path){
  KASSERT(path >= 1);
  KASSERT(path <= 12);
  if(path == 1){
      traffic[4]++;
      traffic[5]++;
      traffic[7]++;
      traffic[8]++;
      traffic[9]++;
      traffic[10]++;
      traffic[11]++;
  }else if(path == 2){
      traffic[4]++;
      traffic[5]++;
      traffic[7]++;
      traffic[10]++;
      traffic[11]++;
      traffic[12]++;
  }else if(path == 3){
      traffic[5]++;
      traffic[7]++;
  }else if(path == 4){
      traffic[7]++;
      traffic[8]++;
      traffic[10]++;
      traffic[11]++;
      traffic[12]++;
      traffic[1]++;
      traffic[2]++;
  }else if(path == 5){
      traffic[7]++;
      traffic[8]++;
      traffic[10]++;
      traffic[1]++;
      traffic[2]++;
      traffic[3]++;
  }else if(path == 6){
      traffic[8]++;
      traffic[10]++;
  }else if(path == 7){
      traffic[10]++;
      traffic[11]++;
      traffic[1]++;
      traffic[2]++;
      traffic[3]++;
      traffic[4]++;
      traffic[5]++;
  }else if(path == 8){
      traffic[10]++;
      traffic[11]++;
      traffic[1]++;
      traffic[4]++;
      traffic[5]++;
      traffic[6]++;
  }else if(path == 9){
      traffic[11]++;
      traffic[1]++;
  }else if(path == 10){
      traffic[1]++;
      traffic[2]++;
      traffic[4]++;
      traffic[5]++;
      traffic[6]++;
      traffic[7]++;
      traffic[8]++;
  }else if(path == 11){
      traffic[1]++;
      traffic[2]++;
      traffic[4]++;
      traffic[7]++;
      traffic[8]++;
      traffic[9]++;
  }else if(path == 12){
      traffic[2]++;
      traffic[4]++;
  }
}

static void clearPaths(int path){
  KASSERT(path >= 1);
  KASSERT(path <= 12);
  if(path == 1){
      traffic[4]--;
      traffic[5]--;
      traffic[7]--;
      traffic[8]--;
      traffic[9]--;
      traffic[10]--;
      traffic[11]--;
  }else if(path == 2){
      traffic[4]--;
      traffic[5]--;
      traffic[7]--;
      traffic[10]--;
      traffic[11]--;
      traffic[12]--;
  }else if(path == 3){
      traffic[5]--;
      traffic[7]--;
  }else if(path == 4){
      traffic[7]--;
      traffic[8]--;
      traffic[10]--;
      traffic[11]--;
      traffic[12]--;
      traffic[1]--;
      traffic[2]--;
  }else if(path == 5){
      traffic[7]--;
      traffic[8]--;
      traffic[10]--;
      traffic[1]--;
      traffic[2]--;
      traffic[3]--;
  }else if(path == 6){
      traffic[8]--;
      traffic[10]--;
  }else if(path == 7){
      traffic[10]--;
      traffic[11]--;
      traffic[1]--;
      traffic[2]--;
      traffic[3]--;
      traffic[4]--;
      traffic[5]--;
  }else if(path == 8){
      traffic[10]--;
      traffic[11]--;
      traffic[1]--;
      traffic[4]--;
      traffic[5]--;
      traffic[6]--;
  }else if(path == 9){
      traffic[11]--;
      traffic[1]--;
  }else if(path == 10){
      traffic[1]--;
      traffic[2]--;
      traffic[4]--;
      traffic[5]--;
      traffic[6]--;
      traffic[7]--;
      traffic[8]--;
  }else if(path == 11){
      traffic[1]--;
      traffic[2]--;
      traffic[4]--;
      traffic[7]--;
      traffic[8]--;
      traffic[9]--;
  }else if(path == 12){
      traffic[2]--;
      traffic[4]--;
  } 
}



/*
    give a unique number to different paths
*/
static int directionsToPath(Direction origin, Direction destination){

    int returnVal;
    if(origin == destination){
        panic("Error: origin and destination are the same");
    }
    if(origin == west){
        if(destination == north){
            returnVal = 1;
        }else if(destination == east){
            returnVal = 2;
        }else if(destination == south){
            returnVal = 3;
        }
    }else if(origin == north){
        if(destination == east){
            returnVal = 4;
        }else if(destination == south){
            returnVal = 5;
        }else if(destination == west){
            returnVal = 6;
        }
    }else if(origin == east){
        if(destination == north){
            returnVal = 9;
        }else if(destination == west){
            returnVal = 8;
        }else if(destination == south){
            returnVal = 7;
        }
    }else{
        if(destination == north){
            returnVal = 11;
        }else if(destination == east){
            returnVal = 12;
        }else if(destination == west){
            returnVal = 10;
        }
    }
    return returnVal;
}
/*
 * The simulation driver will call this function each time a vehicle
 * tries to enter the intersection, before it enters.
 * This function should cause the calling simulation thread 
 * to block until it is OK for the vehicle to enter the intersection.
 *
 * parameters:
 *    * origin: the Direction from which the vehicle is arriving
 *    * destination: the Direction in which the vehicle is trying to go
 *
 * return value: none
 */

void
intersection_before_entry(Direction origin, Direction destination) 
{
  /* replace this default implementation with your own implementation */
  // (void)origin;  /* avoid compiler complaint about unused parameter */
  // (void)destination;  avoid compiler complaint about unused parameter 
  // KASSERT(intersectionSem != NULL);
  // P(intersectionSem);
  int path;
  path = directionsToPath(origin, destination);
  lock_acquire(trafficLock);
  while(traffic[path] != 0){
    lock_release(trafficLock);
    thread_yield();
    lock_acquire(trafficLock);
  }
  
  blockPaths(path);
  lock_release(trafficLock);
}


/*
 * The simulation driver will call this function each time a vehicle
 * leaves the intersection.
 *
 * parameters:
 *    * origin: the Direction from which the vehicle arrived
 *    * destination: the Direction in which the vehicle is going
 *
 * return value: none
 */

void
intersection_after_exit(Direction origin, Direction destination) 
{
  /* replace this default implementation with your own implementation */
  // (void)origin;   avoid compiler complaint about unused parameter 
  // (void)destination;  avoid compiler complaint about unused parameter 
  // KASSERT(intersectionSem != NULL);
  // V(intersectionSem);

  int path = directionsToPath(origin, destination);
  lock_acquire(trafficLock);
  clearPaths(path);
  lock_release(trafficLock);
}
