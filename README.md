# Simulation of traffic flow through a 4-way intersection 🚦
* Keywords: Templated class, Priority queue, Event-driven simulation

## Overview
This project involves implementation of an event-driven simulation of traffic flow through a 4-way intersection that is managed by a traffic light. The simulation includes cars arriving at the intersection traveling in all four directions, as well as the light changing state throughout the simulation. After the simulation is complete,some statistics about the traffic flow will be output.

## Discription

### Event Handling Overview
Events are a central part of an event-drive simulation, so an EventClass is needed. Event objects will be stored in a SortedListClass that is referred to as “the event list”. The event list will only contain events that are scheduled to occur in the future – it will NOT store events that have already been handled.

Handling an event will often generate additional events to be scheduled to occur in the future. Additionally, handling an event will often change the state of the simulation (i.e. advance the simulation’s “current time” to the time the event being handled at occurs, etc.), and will often change accumulating statistics (like maximum values, counts of objects, etc.). To keep the simulation less complex, we will implement the cars advancing through an intersection only when the light is changing.

Specifically, let’s say the light is green in the north-south direction for 10 time tics. When we handle an event that represents a change in the light to being yellow in the north-south direction, we will implement the logic to advance up to 10 cars in the north-bound queue to travel through the intersection and up to 10 cars in the south-bound queue to travel through the intersection. The idea behind this is that when a light is changing to yellow, all the cars that would want to advance through the intersection on the green light will have arrived and be placed in the queues. So it is during the handling of the change to yellow that we advance the cars for the green light.

Similarly, when we handle an event representing the light changing from yellow in the north-south direction to green in the east-west direction, that is when we will advance cars through the intersection in the north-south direction for the yellow light that is changing.

### Light Cycle
The light will always start as a green light in the east-west direction for our simulation!
The traffic light cycles as follows:
![traffic light cycles](https://github.com/EricLiuuu/simulation-of-traffic-flow-through-a-4-way-intersection/blob/main/Docs/traffic_light_cycle.PNG)

When advancing cars through a green light, cars advance one-per-time-tic. A green light that lasts for 20 time tics will allow up to 20 waiting cars in both directions to advance.

When advancing cars through a yellow light, we must consider that some drivers will advance through a yellow light, but others will stop at the light. For our simulation, we will determine a driver’s decision using a uniform random number such that a specified percentage of drivers will be expected to advance through the light and the remainder will be expected to stop. Obviously, if a driver decides to stop at the yellow light, cars that are behind it will be *forced to stop* as well. Let’s say the simulation parameter that controls this is set to 75. This would mean that 75% of drivers will advance through the yellow light, while 25% will choose to stop at the light. If you request a uniform random number between 1 and 100, you can determine the driver will advance if the random number is less than or equal to 75, while a result greater than 75 will indicate the driver will NOT advance through the light. This is just an example, and you would instead compare your uniform random number to the corresponding simulation control parameter instead of always using the literal value 75. If a driver decides to advance through a yellow light, the following driver will make their own decision, so you’ll draw another uniform random number as needed for that driver, and so on.

### Event Handling Specifics
There are 8 types of events that can occur in the simulation.

1.  Car arrival east-bound (EVENT_ARRIVE_EAST)

    When handling an event of a car arriving at the intersection heading east, it gets added to the east-bound queue of cars waiting to get through the intersection. The next east-bound arrival event is also scheduled during the handling of this type of event, based on the east-bound arrival distribution parameters (mean and standard deviation).
    
2. Car arrival west-bound (EVENT_ARRIVE_WEST)

    Same as “Car arrival east-bound” but for a west-bound car instead.
3. Car arrival north-bound (EVENT_ARRIVE_NORTH)

    Same as “Car arrival east-bound” but for a north-bound car instead.
4. Car arrival south-bound (EVENT_ARRIVE_SOUTH)

    Same as “Car arrival east-bound” but for a south-bound car instead.
5. Light change from “Green in East-West” to “Yellow in East-West” (EVENT_CHANGE_YELLOW_EW)

    When the light changes FROM a green light in the east-west direction TO a yellow light in the east-west direction, you will advance cars through the intersection for the green light in the east-west direction. If the light is green in the east-west direction for 10 time tics, then when handling this event type, you will allow up to 10 cars waiting in the east-bound queue to advance through the intersection (or less than 10 if there are less than 10 cars waiting in the queue), and up to 10 cars waiting in the west-bound queue to advance through the intersection. Finally, you’ll also schedule the next light change event to occur at the appropriate time in the future (the next light change event in this case would be a light change from “Yellow in East-West” to “Green in North-South”.
6. Light change from “Yellow in East-West” to “Green in North-South” (EVENT_CHANGE_GREEN_NS)

    Advance cars through the intersection for the yellow light in the east-west direction, and schedule the next light change (from “Green in North-South” to “Yellow in North-South”
7. Light change from “Green in North-South” to “Yellow in North-South” (EVENT_CHANGE_YELLOW_NS)

    Advance cars through the intersection for the green light in the north-soth direction, and schedule the next light change (from “Yellow in North-South” to “Green in East-West”
8. Light change from “Yellow in North-South” to “Green in East-West” (EVENT_CHANGE_GREEN_EW)

    Advance cars through the intersection for the yellow light in the north-south direction, and schedule the next light change (from “Green in East-West” to “Yellow in East-West”

## Parameters and Input File Format
There are 15 control parameters that will affect how the simulation performs. To make it easy to run many different simulations, the simulation control parameters will be read from a text file, whose name will be provided to the program via a command line argument. The parameters are as follows:

* Random number generator seed: A non-negative integer value for seeding the random number generator to select a pseudo-random sequence for the simulation to use.
* Simulation end time: A positive integer value indicating when the simulation should end. When the next event occurs after this specified number of time tics, the event, and any scheduled after it will not be handled, and the simulation will be deemed completed.
* East-west green time and east-west yellow time: Two positive integers indicating the amount of time the light is green and yellow in the east-west direction. Red duration is NOT specified, as it is assumed to be the sum of the green and yellow times for the north-south direction.
* North-south green time and north-south yellow time: Two positive integers indicating the amount of time the light is green and yellow in the north-south direction. Red duration is NOT specified, as it is assumed to be the sum of the green and yellow times for the east-west direction.
* Mean and standard deviation for the arrival rate of east-bound cars: Two positive floating point values indicating the mean and standard deviation of the pre-determined normal distribution characterizing the arrival rate for cars arriving at the intersection in an east-bound direction.
* Mean and standard deviation for the arrival rate of west-bound cars: Two positive floating point values indicating the mean and standard deviation of the pre-determined normal distribution characterizing the arrival rate for cars arriving at the intersection in an west-bound direction.
* Mean and standard deviation for the arrival rate of north-bound cars: Two positive floating point values indicating the mean and standard deviation of the pre-determined normal distribution characterizing the arrival rate for cars arriving at the intersection in an north-bound direction.
* Mean and standard deviation for the arrival rate of south-bound cars: Two positive floating point values indicating the mean and standard deviation of the pre-determined normal distribution characterizing the arrival rate for cars arriving at the intersection in an south-bound direction.
* Percentage of drivers that will advance through a yellow light: An integer value from 0 to 100 (inclusive) describing how often a driver will choose to advance through the intersection when the light is yellow. A number of 40 indicates that 40% of the time a driver will choose to advance through a yellow light, while 60% of the time a driver will choose to stop at the yellow light.


The input file shall be a simple ASCII file with the 15 values specified in a whitespace-delimited way, in the exact order specified above. An example input file follows:
```
12345
1000
10  2
17  3
3  1.04
15  2.7
7.25  0
12.5  3.3
65
```

This input file indicates the random number will be seeded with the value 12345, the simulation will end when the simulation clock reaches 1000 time tics, the east-west light will remain green for 10 time tics and yellow for 2 time tics, the north-south light will remain green for 17 time tics and yellow for 3 time tics, cars will arrive east-bound via a normal distribution with a mean of 3 time tics and a standard deviation of 1.04, cars will arrive west-bound via a normal distribution with a mean of 15 time tics and a standard deviation of 2.7, cars will arrive north-bound via a normal distribution with a mean of 7.25 time tics and a standard deviation of 0, cars will arrive south-bound via a normal distribution with a mean of 12.5 time tics and a standard deviation of 3.3, and on average, 65% of drivers will choose to advance through the intersection at a yellow light.

## Usage

Run the executable with an argument of the parameter file (please follow the previous section for the input file format):
```bash
bash-4.2$ ./proj5.exe paramsA.txt
```

If success, the terminal will display the folllowing information and start the process:
```
Reading parameters from file: paramsA.txt
Parameters read in successfully - simulation is ready!
===== Begin Simulation Parameters =====
  Random generator seed: 12345
  Simulation end time: 100
  East-West Timing - Green: 10 Yellow: 2 Red: 23
  North-South Timing - Green: 20 Yellow: 3 Red: 12
  Arrival Distributions:
    East - Mean: 2 StdDev: 0
    West - Mean: 9 StdDev: 0
    North - Mean: 4 StdDev: 0
    South - Mean: 9 StdDev: 0
  Percentage cars advancing through yellow: 0
===== End Simulation Parameters =====

Schedule simulation seed events
Time: 0 Scheduled Event Type: East-Bound Arrival Time: 2
Time: 0 Scheduled Event Type: West-Bound Arrival Time: 9
Time: 0 Scheduled Event Type: North-Bound Arrival Time: 4
Time: 0 Scheduled Event Type: South-Bound Arrival Time: 9
Time: 0 Scheduled Event Type: Light Change to EW Yellow Time: 10

Starting simulation!

Handling Event Type: East-Bound Arrival Time: 2
Time: 2 Car #0 arrives east-bound - queue length: 1
Time: 2 Scheduled Event Type: East-Bound Arrival Time: 4

Handling Event Type: North-Bound Arrival Time: 4
Time: 4 Car #1 arrives north-bound - queue length: 1
Time: 4 Scheduled Event Type: North-Bound Arrival Time: 8

Handling Event Type: East-Bound Arrival Time: 4
Time: 4 Car #2 arrives east-bound - queue length: 2
Time: 4 Scheduled Event Type: East-Bound Arrival Time: 6

...

Handling Event Type: Light Change to EW Yellow Time: 10
Advancing cars on east-west green
  Car #0 advances east-bound
  Car #2 advances east-bound
  Car #3 advances east-bound
  Car #5 advances east-bound
  Car #6 advances west-bound
East-bound cars advanced on green: 4 Remaining queue: 0
West-bound cars advanced on green: 1 Remaining queue: 0
Time: 10 Scheduled Event Type: Light Change to NS Green Time: 12

...


Next event occurs AFTER the simulation end time (Event Type: Light Change to NS Yellow Time: 102)!
Simulation ran successfully!
===== Begin Simulation Statistics =====
  Longest east-bound queue: 26
  Longest west-bound queue: 4
  Longest north-bound queue: 9
  Longest south-bound queue: 4
  Total cars advanced east-bound: 24
  Total cars advanced west-bound: 8
  Total cars advanced north-bound: 16
  Total cars advanced south-bound: 7
===== End Simulation Statistics =====
```


## Acknowledgments

* [Morgan, Andrew](https://cse.engin.umich.edu/people/faculty/instructional-faculty/)

  Instructional faculty at University of Michigan - Ann Arbor, department of EECS
