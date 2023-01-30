#include "event.h"

Event* createEvent( eventType eventType, int timeStepToExecute, int from, int to ) {
    Event* e;

    e = ( Event* ) malloc( sizeof( Event ) );

    e->type = eventType;
    e->timeStep = timeStepToExecute;
    e->from = from;
    e->to = to;
    e->numCars = 0;
    e->carQueue = createQueue();

    return e;
}

void addCarToEventQueue( Event* e, Car* c) {
    enqueue( e->carQueue, c );
    (e->numCars)++;
}