#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "IntersectionSimulationClass.h"
#include "random.h"

void IntersectionSimulationClass::readParametersFromFile(
     const string &paramFname
     )
{
  bool success = true;;
  ifstream paramF;

  paramF.open(paramFname.c_str());

  //Check that the file was able to be opened...
  if (paramF.fail())
  {
    success = false;
    cout << "ERROR: Unable to open parameter file: " << paramFname << endl;
  }
  else
  {
    //Now read in all the params, according to the specified format of
    //the text-based parameter file.
    if (success)
    {
      paramF >> randomSeedVal;
      if (paramF.fail() ||
          randomSeedVal < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set random generatsor seed" << endl;
      }
    }

    if (success)
    {
      paramF >> timeToStopSim;
      if (paramF.fail() ||
          timeToStopSim <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set simulation end time" << endl;
      }
    }

    if (success)
    {
      paramF >> eastWestGreenTime >> eastWestYellowTime;
      if (paramF.fail() ||
          eastWestGreenTime <= 0 ||
          eastWestYellowTime <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set east-west times" << endl;
      }
    }

    if (success)
    {
      paramF >> northSouthGreenTime >> northSouthYellowTime;
      if (paramF.fail() ||
          northSouthGreenTime <= 0 ||
          northSouthYellowTime <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set north-south times" << endl;
      }
    }

    if (success)
    {
      paramF >> eastArrivalMean >> eastArrivalStdDev;
      if (paramF.fail() ||
          eastArrivalMean <= 0 ||
          eastArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set east arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> westArrivalMean >> westArrivalStdDev;
      if (paramF.fail() ||
          westArrivalMean <= 0 ||
          westArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set west arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> northArrivalMean >> northArrivalStdDev;
      if (paramF.fail() ||
          northArrivalMean <= 0 ||
          northArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set north arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> southArrivalMean >> southArrivalStdDev;
      if (paramF.fail() ||
          southArrivalMean <= 0 ||
          southArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set south arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> percentCarsAdvanceOnYellow;
      if (paramF.fail() || 
          percentCarsAdvanceOnYellow < 0 ||
          percentCarsAdvanceOnYellow > 100)
      {
        success = false;
        cout << "ERROR: Unable to read/set percentage yellow advance" << endl;
      }

      //Use the specified seed to seed the random number generator
      setSeed(randomSeedVal);
    }

    paramF.close();
  }
   
  //Let the caller know whether things went well or not by printing the
  if (!success)
  {
    cout << "ERROR: Parameter file was NOT read in successfully, so the " <<
            "simulation is NOT setup properly!" << endl;
    isSetupProperly = false;
  }
  else
  {
    cout << "Parameters read in successfully - simulation is ready!" << endl;
    isSetupProperly = true;
  }
}

void IntersectionSimulationClass::printParameters() const
{
  cout << "===== Begin Simulation Parameters =====" << endl;
  if (!isSetupProperly)
  {
    cout << "  Simulation is not yet properly setup!" << endl;
  }
  else
  {
    cout << "  Random generator seed: " << randomSeedVal << endl;
    cout << "  Simulation end time: " << timeToStopSim << endl;

    cout << "  East-West Timing -" <<
            " Green: " << eastWestGreenTime << 
            " Yellow: " << eastWestYellowTime <<
            " Red: " << getEastWestRedTime() << endl;

    cout << "  North-South Timing -" <<
            " Green: " << northSouthGreenTime << 
            " Yellow: " << northSouthYellowTime <<
            " Red: " << getNorthSouthRedTime() << endl;

    cout << "  Arrival Distributions:" << endl;
    cout << "    East - Mean: " << eastArrivalMean << 
            " StdDev: " << eastArrivalStdDev << endl;
    cout << "    West - Mean: " << westArrivalMean << 
            " StdDev: " << westArrivalStdDev << endl;
    cout << "    North - Mean: " << northArrivalMean << 
            " StdDev: " << northArrivalStdDev << endl;
    cout << "    South - Mean: " << southArrivalMean << 
            " StdDev: " << southArrivalStdDev << endl;

    cout << "  Percentage cars advancing through yellow: " <<
            percentCarsAdvanceOnYellow << endl;
  }
  cout << "===== End Simulation Parameters =====" << endl;
}

void IntersectionSimulationClass::scheduleArrival(
     const string& travelDir
     )
{
  EventClass newEvent;
  CarClass newCar;
  int timeOccurs;

  if (travelDir == EAST_DIRECTION)
  {
    timeOccurs = getPositiveNormal(eastArrivalMean, eastArrivalStdDev);
    newEvent = EventClass(currentTime + timeOccurs, EVENT_ARRIVE_EAST);
    
    
    if (currentTime != INITIAL_TIME)
    {
      newCar = CarClass(EAST_DIRECTION, currentTime);
      eastQueue.enqueue(newCar);
      cout << "Time: " << currentTime << " Car #" << newCar.getId()
        << " arrives east-bound - queue length: " << eastQueue.getNumElems() 
        << endl;
      if (eastQueue.getNumElems() > maxEastQueueLength)
      {
        maxEastQueueLength ++;
      }
    }
  }
  else if (travelDir == WEST_DIRECTION)
  {
    timeOccurs = getPositiveNormal(westArrivalMean, westArrivalStdDev);
    newEvent = EventClass(currentTime + timeOccurs, EVENT_ARRIVE_WEST);
    
    
    if (currentTime != INITIAL_TIME)
    {
      newCar = CarClass(WEST_DIRECTION, currentTime);
      westQueue.enqueue(newCar);
      cout << "Time: " << currentTime << " Car #" << newCar.getId()
        << " arrives west-bound - queue length: " << westQueue.getNumElems()
        << endl;
      if (westQueue.getNumElems() > maxWestQueueLength)
      {
        maxWestQueueLength ++;
      }
    }
  }
  else if (travelDir == NORTH_DIRECTION)
  {
    timeOccurs = getPositiveNormal(northArrivalMean, northArrivalStdDev);
    newEvent = EventClass(currentTime + timeOccurs, EVENT_ARRIVE_NORTH);
    
    
    if (currentTime != INITIAL_TIME)
    {
      newCar = CarClass(NORTH_DIRECTION, currentTime);
      northQueue.enqueue(newCar);
      cout << "Time: " << currentTime << " Car #" << newCar.getId()
        << " arrives north-bound - queue length: " << northQueue.getNumElems()
        << endl;
      if (northQueue.getNumElems() > maxNorthQueueLength)
      {
        maxNorthQueueLength ++;
      }
    }
  }
  else if (travelDir == SOUTH_DIRECTION)
  {
    timeOccurs = getPositiveNormal(southArrivalMean, southArrivalStdDev);
    newEvent = EventClass(currentTime + timeOccurs, EVENT_ARRIVE_SOUTH);
    
    
    if (currentTime != INITIAL_TIME)
    {
      newCar = CarClass(SOUTH_DIRECTION, currentTime);
      southQueue.enqueue(newCar);
      cout << "Time: " << currentTime << " Car #" << newCar.getId()
        << " arrives south-bound - queue length: " << southQueue.getNumElems()
        << endl;
      if (southQueue.getNumElems() > maxSouthQueueLength)
      {
        maxSouthQueueLength ++;
      }
    }
  }
  else
  {
    cout << "Unknown Arrival condition." << endl;
  }
  
  cout << "Time: " << currentTime << " Scheduled ";
  eventList.insertValue(newEvent);
  cout << newEvent << endl;
  if (currentTime != INITIAL_TIME)
  {
    cout << endl;
  }
}

void IntersectionSimulationClass::scheduleLightChange(
     )
{
  EventClass newEvent;
  if (currentLight == LIGHT_GREEN_EW)
  {
    newEvent = EventClass(currentTime + eastWestGreenTime,
                          EVENT_CHANGE_YELLOW_EW);
    currentLight = LIGHT_YELLOW_EW;
  }
  else if (currentLight == LIGHT_YELLOW_EW)
  {
    newEvent = EventClass(currentTime + eastWestYellowTime,
                          EVENT_CHANGE_GREEN_NS);
    currentLight = LIGHT_GREEN_NS;
  }
  else if (currentLight == LIGHT_GREEN_NS)
  {
    newEvent = EventClass(currentTime + northSouthGreenTime,
                          EVENT_CHANGE_YELLOW_NS);
    currentLight = LIGHT_YELLOW_NS;
  }
  else if (currentLight == LIGHT_YELLOW_NS)
  {
    newEvent = EventClass(currentTime + northSouthYellowTime,
                          EVENT_CHANGE_GREEN_EW);
    currentLight = LIGHT_GREEN_EW;
  }
  eventList.insertValue(newEvent);
  cout << "Time: " << currentTime << " Scheduled " << newEvent << endl;
  cout << endl;
}

bool IntersectionSimulationClass::handleNextEvent(
     )
{
  EventClass currentEvent;
  EventClass removedEvent;
  int typeID;
  int firstNumOfAdvancedCars;
  int secondNumOfAdvancedCars;
  
  if (eventList.getElemAtIndex(FIRST_ELEMENT, currentEvent))
  {
    currentTime = currentEvent.getTimeOccurs();
    
    if (currentTime <= timeToStopSim)
    {
      cout << "Handling " << currentEvent << endl;
      typeID = currentEvent.getType();

      if (typeID == EVENT_ARRIVE_EAST)
      {
        scheduleArrival(EAST_DIRECTION);
      }
      else if (typeID == EVENT_ARRIVE_WEST)
      {
        scheduleArrival(WEST_DIRECTION);
      }
      else if (typeID == EVENT_ARRIVE_NORTH)
      {
        scheduleArrival(NORTH_DIRECTION);
      }
      else if (typeID == EVENT_ARRIVE_SOUTH)
      {
        scheduleArrival(SOUTH_DIRECTION);
      }
      else if (typeID == EVENT_CHANGE_GREEN_EW)
      {
        firstNumOfAdvancedCars = advanceCarsOnYellow(NORTH_DIRECTION);
        secondNumOfAdvancedCars = advanceCarsOnYellow(SOUTH_DIRECTION);

        cout << "North-bound cars advanced on yellow: " <<
          firstNumOfAdvancedCars << " Remaining queue: " <<
          northQueue.getNumElems() << endl;
        cout << "South-bound cars advanced on yellow: " <<
          secondNumOfAdvancedCars << " Remaining queue: " <<
          southQueue.getNumElems() << endl;

        scheduleLightChange();
      }
      else if (typeID == EVENT_CHANGE_YELLOW_EW)
      {
        firstNumOfAdvancedCars = advanceCarsOnGreen(EAST_DIRECTION);
        secondNumOfAdvancedCars = advanceCarsOnGreen(WEST_DIRECTION);

        cout << "East-bound cars advanced on green: " << firstNumOfAdvancedCars
          << " Remaining queue: " << eastQueue.getNumElems() << endl;
        cout << "West-bound cars advanced on green: " << secondNumOfAdvancedCars 
          <<" Remaining queue: " << westQueue.getNumElems() << endl;
        scheduleLightChange();
      }
      else if (typeID == EVENT_CHANGE_GREEN_NS)
      {
        firstNumOfAdvancedCars = advanceCarsOnYellow(EAST_DIRECTION);
        secondNumOfAdvancedCars = advanceCarsOnYellow(WEST_DIRECTION);

        cout << "East-bound cars advanced on yellow: " <<
          firstNumOfAdvancedCars << " Remaining queue: " <<
          eastQueue.getNumElems() << endl;
        cout << "West-bound cars advanced on yellow: " <<
          secondNumOfAdvancedCars << " Remaining queue: " <<
          westQueue.getNumElems() << endl;

        scheduleLightChange();
      }
      else if (typeID == EVENT_CHANGE_YELLOW_NS)
      {
        firstNumOfAdvancedCars = advanceCarsOnGreen(NORTH_DIRECTION);
        secondNumOfAdvancedCars = advanceCarsOnGreen(SOUTH_DIRECTION);

        cout << "North-bound cars advanced on green: " <<
          firstNumOfAdvancedCars << " Remaining queue: " <<
          northQueue.getNumElems() << endl;
        cout << "South-bound cars advanced on green: " << 
          secondNumOfAdvancedCars << " Remaining queue: " << 
          southQueue.getNumElems() << endl;

        scheduleLightChange();
      }

      eventList.removeFront(removedEvent);
      return true;
    }
    else
    {
      cout << "Next event occurs AFTER the simulation end time ("
        << currentEvent << ")!" << endl;
      return false;
    }
  }
  else
  {
    cout << "Empty list." << endl;
    return false;
  }
}

void IntersectionSimulationClass::printStatistics(
     ) const
{
  cout << "===== Begin Simulation Statistics =====" << endl;
  cout << "  Longest east-bound queue: " << maxEastQueueLength << endl;
  cout << "  Longest west-bound queue: " << maxWestQueueLength << endl;
  cout << "  Longest north-bound queue: " << maxNorthQueueLength << endl;
  cout << "  Longest south-bound queue: " << maxSouthQueueLength << endl;
  cout << "  Total cars advanced east-bound: " <<
          numTotalAdvancedEast << endl;
  cout << "  Total cars advanced west-bound: " <<
          numTotalAdvancedWest << endl;
  cout << "  Total cars advanced north-bound: " <<
          numTotalAdvancedNorth << endl;
  cout << "  Total cars advanced south-bound: " <<
          numTotalAdvancedSouth << endl;
  cout << "===== End Simulation Statistics =====" << endl;
}


int IntersectionSimulationClass::advanceCarsOnGreen(const string& travelDir)
{
  CarClass advancedCar;
  int numOfAdvancedCars = 0;
  bool isContinue = true;

  if (travelDir == EAST_DIRECTION)
  {
    cout << "Advancing cars on east-west green" << endl;
    while (isContinue)
    {
      if (eastQueue.dequeue(advancedCar))
      {
        cout << "  Car #" << advancedCar.getId() << " advances east-bound" <<
          endl;
        numOfAdvancedCars ++;
        numTotalAdvancedEast ++;
      }
      else
      {
        isContinue = false;
      }
      if (numOfAdvancedCars == eastWestGreenTime)
      {
        isContinue = false;
      }
    }
  }
  else if (travelDir == WEST_DIRECTION)
  {
    while (isContinue)
    {
      if (westQueue.dequeue(advancedCar))
      {
        cout << "  Car #" << advancedCar.getId() << " advances west-bound" <<
          endl;
        numOfAdvancedCars ++;
        numTotalAdvancedWest ++;
      }
      else
      {
        isContinue = false;
      }
      if (numOfAdvancedCars == eastWestGreenTime)
      {
        isContinue = false;
      }
    }
  }
  else if (travelDir == NORTH_DIRECTION)
  {
    cout << "Advancing cars on north-south green" << endl;
    while (isContinue)
    {
      if (northQueue.dequeue(advancedCar))
      {
        cout << "  Car #" << advancedCar.getId() << " advances north-bound" <<
          endl;
        numOfAdvancedCars ++;
        numTotalAdvancedNorth ++;
      }
      else
      {
        isContinue = false;
      }
      if (numOfAdvancedCars == northSouthGreenTime)
      {
        isContinue = false;
      }
    }
  }
  else if (travelDir == SOUTH_DIRECTION)
  {
    while (isContinue)
    {
      if (southQueue.dequeue(advancedCar))
      {
        cout << "  Car #" << advancedCar.getId() << " advances south-bound" <<
          endl;
        numOfAdvancedCars ++;
        numTotalAdvancedSouth ++;
      }
      else
      {
        isContinue = false;
      }
      if (numOfAdvancedCars == northSouthGreenTime)
      {
        isContinue = false;
      }
    }
  }
  return numOfAdvancedCars;
}


int IntersectionSimulationClass::advanceCarsOnYellow(const string& travelDir)
{
  CarClass advancedCar;
  int numOfAdvancedCars = 0;
  bool isContinue = true;

  if (travelDir == EAST_DIRECTION)
  {
    cout << "Advancing cars on east-west yellow" << endl;

    while (isContinue)
    {
      if (getUniform(MIN_VALUE, MAX_VALUE) <= percentCarsAdvanceOnYellow)
      {
        if (eastQueue.dequeue(advancedCar))
        {
          cout << "  Next east-bound car will advance on yellow" << endl;
          cout << "  Car #" << advancedCar.getId() << " advances east-bound" <<
            endl;
          numOfAdvancedCars ++;
          numTotalAdvancedEast ++;
        }
        else
        {
          cout << "  No east-bound cars waiting to advance on yellow" << endl;
          isContinue = false;
        }

        if (numOfAdvancedCars == eastWestYellowTime)
        {
          isContinue = false;
        }
      }
      else
      {
        if (eastQueue.getNumElems() == NOTHING_IN_THE_LIST)
        {
          cout << "  No east-bound cars waiting to advance on yellow" << endl;
        }
        else
        {
          cout << "  Next east-bound car will NOT advance on yellow" << endl;
        }
        isContinue = false;
      }
    }
    
  }
  else if (travelDir == WEST_DIRECTION)
  {
    while (isContinue)
    {
      if (getUniform(MIN_VALUE, MAX_VALUE) <= percentCarsAdvanceOnYellow)
      {
        if (westQueue.dequeue(advancedCar))
        {
          cout << "  Next west-bound car will advance on yellow" << endl;
          cout << "  Car #" << advancedCar.getId() << " advances west-bound" <<
            endl;
          numOfAdvancedCars ++;
          numTotalAdvancedWest ++;
        }
        else
        {
          cout << "  No west-bound cars waiting to advance on yellow" << endl;
          isContinue = false;
        }

        if (numOfAdvancedCars == eastWestYellowTime)
        {
          isContinue = false;
        }
      }
      else
      {
        if (westQueue.getNumElems() == NOTHING_IN_THE_LIST)
        {
          cout << "  No west-bound cars waiting to advance on yellow" << endl;
        }
        else
        {
          cout << "  Next west-bound car will NOT advance on yellow" << endl;
        }
        isContinue = false;
      }
    }
    
  }
  else if (travelDir == NORTH_DIRECTION)
  {
    cout << "Advancing cars on north-south yellow" << endl;
    while (isContinue)
    {
      if (getUniform(MIN_VALUE, MAX_VALUE) <= percentCarsAdvanceOnYellow)
      {
        if (northQueue.dequeue(advancedCar))
        {
          cout << "  Next north-bound car will advance on yellow" << endl;
          cout << "  Car #" << advancedCar.getId() << " advances north-bound" <<
            endl;
          numOfAdvancedCars ++;
          numTotalAdvancedNorth ++;
        }
        else
        {
          cout << "  No north-bound cars waiting to advance on yellow" << endl;
          isContinue = false;
        }

        if (numOfAdvancedCars == northSouthYellowTime)
        {
          isContinue = false;
        }
      }
      else
      {
        if (northQueue.getNumElems() == NOTHING_IN_THE_LIST)
        {
          cout << "  No north-bound cars waiting to advance on yellow" << endl;
        }
        else
        {
          cout << "  Next north-bound car will NOT advance on yellow" << endl;
        }
        isContinue = false;
      }
    }
    
  }
  else if (travelDir == SOUTH_DIRECTION)
  {
    while (isContinue)
    {
      if (getUniform(MIN_VALUE, MAX_VALUE) <= percentCarsAdvanceOnYellow)
      {
        if (southQueue.dequeue(advancedCar))
        {
          cout << "  Next south-bound car will advance on yellow" << endl;
          cout << "  Car #" << advancedCar.getId() << " advances south-bound" <<
            endl;
          numOfAdvancedCars ++;
          numTotalAdvancedSouth ++;
        }
        else
        {
          cout << "  No south-bound cars waiting to advance on yellow" << endl;
          isContinue = false;
        }

        if (numOfAdvancedCars == northSouthYellowTime)
        {
          isContinue = false;
        }
      }
      else
      {
        if (southQueue.getNumElems() == NOTHING_IN_THE_LIST)
        {
          cout << "  No south-bound cars waiting to advance on yellow" << endl;
        }
        else
        {
          cout << "  Next south-bound car will NOT advance on yellow" << endl;
        }
        isContinue = false;
      }
    }
  }
  return numOfAdvancedCars;
}