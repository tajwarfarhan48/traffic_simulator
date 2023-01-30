#ifndef _event_h
#define _event_h
#include <stdbool.h>

#include "queue.h"
#include "road.h"

typedef enum eventType{ ADD_CAR_EVENT, PRINT_ROADS_EVENT } eventType;

typedef struct Event
{
    eventType type;
    int timeStep;
    int from;
    int to;
    int numCars;
    Queue* carQueue;

}  Event;

Event* createEvent( eventType eventType, int timeStepToExecute, int from, int to );
void addCarToEventQueue( Event* e, Car* c);

#endif
