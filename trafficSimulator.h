#ifndef _trafficSimulator_h
#define _trafficSimulator_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "graph.h"
#include "queue.h"
#include "priorityQueue.h"
#include "event.h"
#include "road.h"

typedef struct TrafficData
{
    /* graph representing the road network */
    Graph* roadNetwork;

    /* array of the roads of the graph in the sequence they were added to the graph */
    int numRoads;
    RoadData** roadArray;

    /* priority queue of events where the priority represents the time the event will occur */
    PriorityQueue* eventQueue;

    /* track the number of cars still in the simulator */
    int numCarsLeft;
    int numCarsExited;

    /* track the longest number of time steps that any light takes to cycle around.  This is useful for detecting gridlock. */
    int maxCycleLength;

    /* Statistics */
    double avgTime;
    int maxTime;
    int totalTime;

}  TrafficData;

void printNames( );

void trafficSimulator( char* filename );
bool goToNextIntersection( TrafficData* t, RoadData* r, int curCycle );
TrafficData* readTrafficDataFromInputFile( char* filename );

#endif
