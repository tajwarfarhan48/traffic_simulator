#include "trafficSimulator.h"

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 */
void printNames( )
{
    printf("This solution was completed by:\n");
    printf("Farhan Tajwar Romit\n");
    printf("N/A\n");
}

bool goToNextIntersection( TrafficData* t, RoadData* r, int curCycle ) {
    RoadData* curRoad;
    RoadData* nextRoad;
    Car* c;
    bool moved = false;

    int from;
    int to;
    int timeTaken;
    int i;

    c = r->curDrivingCars[0];
    from = r->endIndex;
    to = -1;

    /* The case where the car is absent or cannot move due to a red light */
    if ( r->color == RED || c == NULL ) {
        return moved;
    }

    /* The case where the car has reached its destination */
    else if ( from == c->destination ) {
        moved = true;
        (t->numCarsLeft)--;
        (t->numCarsExited)++;
        timeTaken = curCycle - c->timeStepAdded + 1;
        t->totalTime += timeTaken;

        if ( timeTaken > t->maxTime ) {
            t->maxTime = timeTaken;
        }

        t->avgTime = ( ( double ) t->totalTime ) / t->numCarsExited;

        printf( "CYCLE %d - Car successfully traveled from %d to %d in %d time steps.\n\n", curCycle, c->start, c->destination, timeTaken );

        free( c );

        r->curDrivingCars[0] = NULL;

        return moved;
    }

    /* The case where the car still has not reached its destination */
    else {
        getNextOnShortestPath( t->roadNetwork, from, c->destination, &to );

        if ( to == -1 ) return moved;

        for ( i = 0; i < t->numRoads; i++ ) {
            curRoad = t->roadArray[i];

            if ( curRoad->startIndex == from && curRoad->endIndex == to ) {
                nextRoad = curRoad;
            }
        }

        return changeRoads( r, nextRoad );
    }
}

/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Read in the traffic data from the file whose name is stored in char* filename
 * Simulate the rules outlined in the project 2 description
 */
void trafficSimulator( char* filename )
{
    /* TODO: complete this function */
    Event* curEvent;
    RoadData* r;
    RoadData* curRoad;
    bool moved = false;
    bool temp;

    int iClockCycle = 0;
    int cyclesSinceMoved = 0;
    bool allCarsReached = false;
    bool isGridLock = false;

    int numRoads;
    int curRoadLength;
    int i;

    /* Reads in the traffic data from filename */
    TrafficData* tData = readTrafficDataFromInputFile( filename );

    tData->maxCycleLength = tData->roadArray[0]->cycleResets;

    for ( i = 1; i < tData->numRoads; i++ ) {
        if ( tData->roadArray[i]->cycleResets > tData->maxCycleLength ) {
            tData->maxCycleLength = tData->roadArray[i]->cycleResets;
        }
    }

    /* Loop until all events processed and either all cars reached destination or gridlock has occurred */
    while ( !( isEmptyPQ( tData->eventQueue ) && ( allCarsReached || isGridLock ) ) ) {
        // printf("\n\n\nCurrent Time Step: %d\n\n", iClockCycle);

        for ( i = 0; i < tData->numRoads; i++ ) {
            tData->roadArray[i]->firstCarJustInserted = false;
        }

        /* Update the state of every traffic light */
        for ( i = 0; i < tData->numRoads; i++ ) {
            updateTrafficLight( tData->roadArray[i], iClockCycle );
        }

        /* Loop on events associated with this time step */
        while ( !isEmptyPQ( tData->eventQueue ) &&  getFrontPriority( tData->eventQueue ) == iClockCycle ) {
            curEvent = dequeuePQ( tData-> eventQueue );
            switch ( curEvent->type ) {
                case ADD_CAR_EVENT:
                    printf( "CYCLE %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n\n", iClockCycle, curEvent->from, curEvent->to );

                    for ( i = 0; i < tData->numRoads; i++ ) {
                        r = tData->roadArray[i];

                        if ( r->startIndex == curEvent->from && r->endIndex == curEvent->to) {
                            mergeQueues( r->curWaitingCars, curEvent->carQueue );
                            tData->numCarsLeft += curEvent->numCars;
                        }
                    }

                    break;

                case PRINT_ROADS_EVENT:
                    printf( "CYCLE %d - PRINT_ROADS_EVENT - Current Contents of the roads:\n", iClockCycle );

                    for ( i = 0; i < tData->numRoads; i++ ) {
                        printDestination( tData->roadArray[i] );
                    }

                    printf("\n");

                    break;
            }
        }

        /* Loop on every road */
        for ( i = 0; i < tData->numRoads; i++ ) {
            curRoad = tData->roadArray[i];
            curRoadLength = curRoad->length;

            /* First try to move cars through the next intersection */
            temp = goToNextIntersection( tData, curRoad, iClockCycle );
            moved = temp || moved;
        }

        for ( i = 0; i < tData->numRoads; i++ ) {
            curRoad = tData->roadArray[i];

            /* Second move waiting cars onto the end of each road if possible */
            temp = addCarFromQueue( curRoad );
            moved = temp || moved;
        }

        for ( i = 0; i < tData->numRoads; i++ ) {
            curRoad = tData->roadArray[i];
            curRoadLength = curRoad->length;
            
            /* Third move cars forward on every road (only those that haven't moved yet this time step) */
            if ( curRoad->firstCarJustInserted ) {
                temp = moveCarsForward( curRoad, curRoadLength - 2 );
                moved = temp || moved;
            }

            else {
                temp = moveCarsForward( curRoad, curRoadLength - 1 );
                moved = temp || moved;
            }
        }

        if ( moved ) {
            cyclesSinceMoved = 0;
        }

        else {
            cyclesSinceMoved++;
        }

        allCarsReached = tData->numCarsLeft <= 0;
        isGridLock = cyclesSinceMoved > tData->maxCycleLength;

        if ( isGridLock ) {
            printf("CYCLE %d - Gridlock has been detected.\n", iClockCycle);
        }

        moved = false;
        iClockCycle++;
    }

    if ( !isGridLock ) {
        printf("Average number of time steps to reach their destination is %.2f.\n", tData->avgTime);
        printf("Maximum number of time steps to reach their destination is %d.\n\n", tData->maxTime);
    }

    freeGraph( tData->roadNetwork );

    numRoads = tData->numRoads;

    for ( i = 0; i < numRoads; i++ ) {
        freeRoad( tData->roadArray[i] );
    }

    free( tData->roadArray );

    while ( !isEmptyPQ( tData->eventQueue ) ) {
        curEvent = dequeuePQ( tData->eventQueue );

        while ( !isEmpty( curEvent->carQueue ) ) {
            free( dequeue( curEvent->carQueue ) );
        }

        free( curEvent->carQueue );

        free( curEvent );
    }

    freePQ( tData->eventQueue );

    free( tData );
}


/* readTrafficDataFromInputFile
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData* readTrafficDataFromInputFile( char* filename )
{
    TrafficData* t;
    RoadData* r;
    Event* e;
    Car* c;

    int numVertices;
    int numEdges;

    int numIncomingRoads;

    int fromVertex;
    int roadLength;
    int greenOn;
    int greenOff;
    int cycleResets;

    int numAddCarCommands;

    int startingEdgeFrom;
    int startingEdgeTo;
    int timeStepToAdd;
    int numCarsToAddToEdge;
    int curDestination;

    int numPrintRoadCommands;

    int timeStepToPrint;

    int i;
    int j;
    int k = 0;

    /* open the file */
    FILE *pFile = fopen( filename, "r" );
    t = ( TrafficData* ) malloc( sizeof( TrafficData ) );
    t->eventQueue = createPQ();
    t->numCarsLeft = 0;
    t->numCarsExited = 0;
    t->avgTime = 0;
    t->maxTime = 0;
    t->totalTime = 0;

    fscanf( pFile, "%d %d", &numVertices, &numEdges );
    
    t->roadNetwork = createGraph( numVertices );
    t->roadArray = ( RoadData** ) malloc( numEdges *  sizeof( RoadData* ) );
    t->numRoads = numEdges;

    /* Each road can be stored in a `RoadData` struct (see `road.h`).
     * 
     * Each event will be stored in an Event struct (see `event.h`).
     *
     * Each car is stored in a Car struct (see `car.h`).
     */
    for ( i = 0; i < numVertices; i++ ) {
        fscanf( pFile, "%d", &numIncomingRoads );
        for ( j = 0; j < numIncomingRoads; j++ ) {
            fscanf( pFile, "%d %d\t%d %d %d", &fromVertex, &roadLength, &greenOn, &greenOff, &cycleResets );

            r = createRoad( roadLength, fromVertex, i, greenOn, greenOff, cycleResets );
            setEdge( t->roadNetwork, fromVertex, i, roadLength );
            setEdgeData( t->roadNetwork, fromVertex, i, r );
            t->roadArray[k++] = r;
        }
    }

    fscanf( pFile, "%d", &numAddCarCommands );

    for ( i = 0; i < numAddCarCommands; i++ ) {
        fscanf( pFile, "%d %d %d", &startingEdgeFrom, &startingEdgeTo, &timeStepToAdd );

        e = createEvent( ADD_CAR_EVENT, timeStepToAdd, startingEdgeFrom, startingEdgeTo );

        fscanf( pFile, "%d", &numCarsToAddToEdge );

        for ( j = 0; j < numCarsToAddToEdge - 1; j++ ) {
            fscanf( pFile, "%d ", &curDestination );
            c = createCar( startingEdgeFrom, curDestination, timeStepToAdd );
            addCarToEventQueue( e, c );
        }

        fscanf( pFile, "%d", &curDestination );

        c = createCar( startingEdgeFrom, curDestination, timeStepToAdd );

        addCarToEventQueue( e, c );

        enqueueByPriority( t->eventQueue, e, timeStepToAdd );
    }

    fscanf( pFile, "%d", &numPrintRoadCommands );

    for ( i = 0; i < numPrintRoadCommands; i++ ) {
        fscanf( pFile, "%d", &timeStepToPrint );

        e = createEvent( PRINT_ROADS_EVENT, timeStepToPrint, -1, -1 );

        enqueueByPriority( t->eventQueue, e, timeStepToPrint );
    }

    /* close file */
    fclose( pFile );

    return t;
}
