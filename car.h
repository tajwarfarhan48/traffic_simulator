#ifndef _car_h
#define _car_h
#include <stdlib.h>

typedef struct Car
{
    int timeStepAdded;
    int start;
    int destination;

}  Car;

Car* createCar( int start, int destination, int timeStepAdded );

#endif
