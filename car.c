#include "car.h"


Car* createCar( int start, int destination, int timeStepAdded ) {
    Car* c;

    c = ( Car* ) malloc( sizeof( Car ) );

    c->start = start;
    c->destination = destination;
    c->timeStepAdded = timeStepAdded;

    return c;
}