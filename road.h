#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h"

typedef enum Color { RED, GREEN } Color;

typedef struct RoadData
{
    int length;

    /* information used to record/update whether the light at the end of this road is green or red */
    Color color;
    int greenOn;
    int greenOff;
    int cycleResets;

    /* intersections this road starts from and moves to */
    int startIndex;
    int endIndex;
    int edgeWeight;

    /* Each road should have an array to track cars currently driving on them.  The type of this array can be either `Car**` 
     * (array of car pointers) or `Car*` (array of cars) but the former may be easier to use since locations on the road can 
     * be set to `NULL` if no car is present.
     */
    Car** curDrivingCars;
    bool firstCarJustInserted;
    
    /* For each road it is recommended to have a `queue` of cars waiting to enter the simulated road system (see `queue.h` for 
     * the queues functions which have already been implemented for you).  Cars will be put into this queue only when an add 
     * car event occurs for this road.
     */
    Queue* curWaitingCars;

}  RoadData;

RoadData* createRoad( int roadLength, int startIndex, int endIndex, int greenOn, int greenOff, int cycleResets );
void freeRoad( RoadData* r );
bool moveCarsForward( RoadData* r, int end );
bool changeRoads( RoadData* from, RoadData* to );
bool addCarFromQueue ( RoadData* r );
void updateTrafficLight( RoadData* r, int curCycle );
void printDestination( RoadData* r );
void printCar();

#endif

