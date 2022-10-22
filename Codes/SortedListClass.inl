#ifndef _SORTEDLISTCLASS_INL_
#define _SORTEDLISTCLASS_INL_

#include <iostream>
using namespace std;

#include "SortedListClass.h"

template <class T>
SortedListClass<T>::SortedListClass()
{
  head = NULL;
  tail = NULL;
};

template <class T>
SortedListClass<T>::SortedListClass(const SortedListClass<T>& rhs)
{
  LinkedNodeClass<T>* newNode;
  LinkedNodeClass<T>* tempNext;
  T val;
  if (rhs.head != NULL)
  {
    val = rhs.head->getValue();
    tempNext = rhs.head->getNext();
    newNode = new LinkedNodeClass<T>(NULL, val, NULL);
    head = newNode;
    tail = newNode;

    while (tempNext != NULL)
    {
      val = tempNext->getValue();
      newNode = new LinkedNodeClass<T>(tail, val, NULL);
      newNode->setBeforeAndAfterPointers();
      tail = newNode;
      tempNext = tempNext->getNext();
    }
  }
  else
  {
    head = NULL;
    tail = NULL;
  }
}

template <class T>
SortedListClass<T>::~SortedListClass()
{
  clear();
}



template <class T>
void SortedListClass<T>::clear()
{
  LinkedNodeClass<T>* tempNext;
  LinkedNodeClass<T>* tempPrev;

  if (head != NULL)
  {
    tempNext = head;
    tempPrev = head;
    while (tempNext != NULL)
    {
      tempNext = tempNext->getNext();
      delete tempPrev;
      if (tempNext != NULL)
      {
        tempPrev = tempNext;
      }
    }
    tail = NULL;
    head = NULL;
  }
}

template <class T>
void SortedListClass<T>::insertValue(T& valToInsert)
{
  LinkedNodeClass<T>* newNode;
  LinkedNodeClass<T>* tempPrev;
  LinkedNodeClass<T>* tempNext;
  T localVal;
  bool isContinue = true;

  if (head == NULL && tail == NULL)
  {
    newNode = new LinkedNodeClass<T>(NULL, valToInsert, NULL);
    head = newNode;
    tail = newNode;
  }
  else
  {
    tempPrev = head->getPrev();
    tempNext = head->getNext();
    localVal = head->getValue();

    while (isContinue)
    {
      if (valToInsert < localVal && tempPrev == NULL)
      {
        newNode = new LinkedNodeClass<T>(NULL, valToInsert, head);
        newNode->setBeforeAndAfterPointers();
        head = newNode;
        isContinue = false;
      }
      else if (valToInsert >= localVal && tempNext == NULL)
      {
        newNode = new LinkedNodeClass<T>(tail, valToInsert, NULL);
        newNode->setBeforeAndAfterPointers();
        tail = newNode;
        isContinue = false;
      }
      else if (valToInsert >= localVal && valToInsert < tempNext->getValue())
      {
        newNode = new LinkedNodeClass<T>(tempNext->getPrev(), valToInsert,
                                         tempNext);
        newNode->setBeforeAndAfterPointers();
        isContinue = false;
      }
      else
      {
        tempPrev = tempNext->getPrev();
        localVal = tempNext->getValue();
        tempNext = tempNext->getNext();
      }
    }
  }
}

template <class T>
void SortedListClass<T>::printForward() const
{
  LinkedNodeClass<T>* tempNext;

    cout << "Forward List Contents Follow:" << endl;

    if (head != NULL)
    {
      tempNext = head;
      while (tempNext != NULL)
      {
        cout << "  " << tempNext->getValue() << endl;
        tempNext = tempNext->getNext();
      }
    }
    
    cout << "End Of List Contents" << endl;

}

template <class T>
void SortedListClass<T>::printBackward() const
{
  LinkedNodeClass<T>* tempPrev;

  cout << "Backward List Contents Follow:" << endl;

  if (tail != NULL)
  {
    tempPrev = tail;
    while (tempPrev != NULL)
    {
      cout << "  " << tempPrev->getValue() << endl;
      tempPrev = tempPrev->getPrev();
    }
  }
  cout << "End Of List Contents" << endl;
}

template <class T>
bool SortedListClass<T>::removeFront(T& theVal)
{
  bool isSuccess;
  LinkedNodeClass<T>* tempNext;

  if (head == NULL && tail == NULL)
  {
    isSuccess = false;
  }
  else
  {
    theVal = head->getValue();
    tempNext = head->getNext();
    if (tempNext == NULL)
    {
      delete head;
      head = NULL;
      tail = NULL;
    }
    else
    {
      tempNext->setPreviousPointerToNull();
      delete head;
      head = tempNext;
    }
    isSuccess = true;
  }
  return isSuccess;
}

template <class T>
bool SortedListClass<T>::removeLast(T& theVal)
{
  bool isSuccess;
  LinkedNodeClass<T>* tempPrev;

  if (head == NULL && tail == NULL)
  {
    isSuccess = false;
  }
  else
  {
    theVal = tail->getValue();
    tempPrev = tail->getPrev();
    if (tempPrev == NULL)
    {
      delete tail;
      head = NULL;
      tail = NULL;
    }
    else
    {
      tempPrev->setNextPointerToNull();
      delete tail;
      tail = tempPrev;   
    }
    isSuccess = true;
  }
  return isSuccess;
}

template <class T>
int SortedListClass<T>::getNumElems() const
{
  LinkedNodeClass<T>* tempNext;
  int numOfElements = 0;
  if (head == NULL)
  {
    return numOfElements;
  }
  else
  {
    tempNext = head;
    while (tempNext != NULL)
    {
      numOfElements ++;
      tempNext = tempNext->getNext();
    }
    return numOfElements;
  }
}

template <class T>
bool SortedListClass<T>::getElemAtIndex(const int index, T& outVal) const
{
  LinkedNodeClass<T>* tempNext;
  int currentIndex = 0;
  if (head == NULL)
  {
    return false;
  }
  else
  {
    tempNext = head;
    while (tempNext != NULL)
    {
      if (currentIndex == index)
      {
        outVal = tempNext->getValue();
        return true;
      }
      else
      {
        tempNext = tempNext->getNext();
        currentIndex ++;
      }
    }
    return false;
  }
}

#endif
