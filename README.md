#Traffic Simulator
This is a course project, written in C, that simulates the flow of traffic through a city. To import this project and run it, run the following lines of code in your terminal. (**Important: This project requires the gcc compiler to run**)

`

    git clone https://github.com/tajwarfarhan48/traffic_simulator.git
    cd traffic_simulator
    make
    ./driver
`

##Project Description
The program is broken down into several components:
- **Road Network -** Represented by a graph; the edges of the graph represent the roads, and the vertices represent the intersections between roads
	- Each road in the network can hold a maximum number of cars
	- Each road has a starting and ending intersection (vertex)


- **Cars -** Represents the only type of vehicle in this simulation.
	- Every car has a start and destination intersection
	- Cars travel on roads based on specific rules. They cannot move if another car is in front of them, and they cannot enter a road if the entrance is blocked by another car, or if the road is filled


- **Event Queue -** Represents the sequence of events taking place. Each event has an associated time step (the time the event takes place). There are two types of events in this simulator:
	- Adding new cars to the road
	- Printing existing road contents


##Project Operation
The project follows a pre-determined series of events in a loop:
1. Execute all the events in the event queue associated with the current time step
2. Attempt to move the cars at the front of the roads to the next road, or remove them from the event if they have reached their destination
3. For each road, attempt to add a car to the beginning of that road
4. Attempt to move all the cars in the road one space forward

The project ends either when all cars have reache their destination, or when there is a gridlock in the simulation (no car is able to move for 5 time steps)

A more detailed description of the project is available in the Project Description PDF document in the repository.
