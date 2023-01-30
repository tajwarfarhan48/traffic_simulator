#include "road.h"

RoadData* createRoad( int roadLength, int startIndex, int endIndex, int greenOn, int greenOff, int cycleResets ) {
    RoadData* road;

    road = ( RoadData* ) malloc( sizeof( RoadData ) );

    road->length = roadLength;
    road->color = RED;
    road->startIndex = startIndex;
    road->endIndex = endIndex;
    road->greenOn = greenOn;
    road->greenOff = greenOff;
    road->cycleResets = cycleResets;
    road->firstCarJustInserted = false;
    road->curDrivingCars = ( Car** ) malloc( roadLength * sizeof( Car* ) );
    road->curWaitingCars = createQueue();

    return road;
}

void freeRoad( RoadData* r ) {
    int i;

    for ( i = 0; i < r->length; i++ ) { 
        free( r->curDrivingCars[i] );
    }

    free( r->curDrivingCars );

    while ( !isEmpty( r->curWaitingCars ) ) {
        free( dequeue( r->curWaitingCars ) );
    }

    freeQueue( r->curWaitingCars );

    free( r );
}

bool moveCarsForward( RoadData* r, int end ) {
    int i;
    bool moved = false;

    i = end;
    
    while ( i > 0 ) {
        if ( r->curDrivingCars[i] != NULL && r->curDrivingCars[i-1] == NULL ) {
            moved = true;
            r->curDrivingCars[i-1] = r->curDrivingCars[i];
            r->curDrivingCars[i] = NULL;
            i -= 2;
        }

        else {
            i--;
        }
    }

    return moved;
}

bool changeRoads( RoadData* from, RoadData* to ) {
    bool moved = false;

    if ( to->curDrivingCars[to->length-1] != NULL ) {
        return moved;
    }

    if ( to->firstCarJustInserted ) {
        return moved;
    }

    moved = true;
    to->curDrivingCars[to->length-1] = from->curDrivingCars[0];
    from->curDrivingCars[0] = NULL;
    to->firstCarJustInserted = true;

    return moved;
}

bool addCarFromQueue ( RoadData* r ) {
    int endIndex;
    bool moved = false;

    endIndex = r->length - 1;

    if( r->curDrivingCars[endIndex] != NULL || isEmpty( r->curWaitingCars ) ) return moved;
    
    moved = true;
    r->curDrivingCars[endIndex] = dequeue( r->curWaitingCars );
    r->firstCarJustInserted = true;

    return moved;
}

void updateTrafficLight( RoadData* r, int curCycle ) {
    int lightCycle;

    lightCycle = curCycle % ( r->cycleResets );

    if ( lightCycle < r->greenOn || lightCycle >= r->greenOff ) r->color = RED;
    else r->color = GREEN;
}

void printDestination( RoadData* r ) {
    int i;

    printf("Cars on the road from %d to %d: \n", r->startIndex, r->endIndex);

    for ( i = 0; i < r->length; i++ ) {
        if ( r->curDrivingCars[i] == NULL ) {
            printf("- ");
        }

        else {
            printf("%d ", r->curDrivingCars[i]->destination );
        }
    }

    printf("(%s light)\n", ( r->color == RED ) ? "RED" : "GREEN");
}
