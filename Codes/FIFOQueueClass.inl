#ifndef _FIFOQUEUENODE_INL_
#define _FIFOQUEUENODE_INL_

#include <iostream>
using namespace std;
#include "FIFOQueueClass.h"

template <class T>
FIFOQueueClass<T>::FIFOQueueClass()
{
  head = NULL;
  tail = NULL;
}

template <class T>
FIFOQueueClass<T>::~FIFOQueueClass()
{
  clear();
}

template <class T>
void FIFOQueueClass<T>::enqueue(T& newItem)
{
  
  LinkedNodeClass<T>* newNode;
  if (head == NULL)
  {
    newNode = new LinkedNodeClass<T>(NULL, newItem, NULL);
    head = newNode;
    tail = newNode;
  }
  else
  {
    newNode = new LinkedNodeClass<T>(tail, newItem, NULL);
    newNode->setBeforeAndAfterPointers();
    tail = newNode;
  }
  
}

template <class T>
bool FIFOQueueClass<T>::dequeue(T& outItem)
{
  LinkedNodeClass<T>* tempPtr;
  if (head == NULL)
  {
    return false;
  }
  else
  {
    outItem = head->getValue();
    tempPtr = head->getNext();
    if (tempPtr == NULL)
    {
      delete head;
      head = NULL;
      tail = NULL;
    }
    else
    {
      tempPtr->setPreviousPointerToNull();
      delete head;
      head = tempPtr;
    }
    return true;
  }
}

template <class T>
void FIFOQueueClass<T>::print() const
{
  LinkedNodeClass<T>* tempPtr;

  if (head != NULL)
  {
    tempPtr = head;
    while (tempPtr != NULL)
    {
      cout << tempPtr->getValue() << " ";
      tempPtr = tempPtr->getNext();
    }
    cout << endl;
  }
}

template <class T>
int FIFOQueueClass<T>::getNumElems() const
{
  LinkedNodeClass<T>* tempPtr;
  int numOfElements = 0;
  if (head == NULL)
  {
    return numOfElements;
  }
  else
  {
    tempPtr = head;
    while (tempPtr != NULL)
    {
      numOfElements++;
      tempPtr = tempPtr->getNext();
    }
    return numOfElements;
  }
}

template <class T>
void FIFOQueueClass<T>::clear()
{
  LinkedNodeClass<T>* tempPtr;
  if (head != NULL)
  {
    while (head != NULL)
    {
      tempPtr = head->getNext();
      delete head;
      head = tempPtr;
    }
    tail = NULL;
    head = NULL;
  }
}


#endif
