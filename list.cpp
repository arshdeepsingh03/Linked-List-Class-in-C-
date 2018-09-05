#ifndef LIST_CPP
#define LIST_CPP

#include "list.h"
#include "debug.h"

// constructors, destructors and = operator ======================================================================================

// default constructor
template< typename DATATYPE >
List< DATATYPE >::List() 
   : firstPtr( 0 ), 
     lastPtr( 0 ),
     currentListLength( 0 ) 
{
    #ifdef DEBUG_LIST
        cout << "List C called on " << this << endl;
    #endif 
}

// destructor
template< typename DATATYPE >
List< DATATYPE >::~List() 
{
    #ifdef DEBUG_LIST
        cout << "List D called on " << this << endl;
    #endif 
    
    deleteTrailingNodes( firstPtr );
}

// copy constructor
template< typename DATATYPE >
List< DATATYPE >::List( List< DATATYPE >& listToCopy ) 
    : currentListLength( listToCopy.currentListLength ) 
{ 
    #ifdef DEBUG_LIST
        cout << "List CC called on " << this << endl;
    #endif 

    if( listToCopy.isEmpty() ) 
        firstPtr = lastPtr = 0;
    else 
    {
        firstPtr = getNewNode( (listToCopy.firstPtr)->data );

        ListNode< DATATYPE >* readPtr = (listToCopy.firstPtr)->nextPtr;
        // write lags behind because previous node's nextptr must be written
        ListNode< DATATYPE >* writePtr = firstPtr;

        while( readPtr != 0 ) 
        {
            writePtr->nextPtr = getNewNode( readPtr->data );
            writePtr = writePtr->nextPtr;
            readPtr = readPtr->nextPtr;
        }

        lastPtr = writePtr;
        lastPtr->nextPtr = 0;
    }
}

// assignment operator
template< typename DATATYPE >
const List< DATATYPE >& List< DATATYPE >::operator=( const List< DATATYPE >& right )
{
    #ifdef DEBUG_LIST
        cout << "List op= called on " << this << endl;
    #endif 

    if( right.isEmpty() )
    {
        deleteTrailingNodes( firstPtr );
        firstPtr = lastPtr = 0;  
    }
    else
    {
        ListNode< DATATYPE >* readPtr = right.firstPtr;
        ListNode< DATATYPE >* writePtr = firstPtr;

        if( isEmpty() )
        {
            writePtr = firstPtr = getNewNode( readPtr->data );
            advanceNodePtr( readPtr );
        }

        while( readPtr != 0 )
        {
            if( isThereNodeAhead( writePtr ) )
                writePtr->data = readPtr->data;
            else
                writePtr->nextPtr = getNewNode( readPtr->data );
            
            advanceNodePtr( readPtr );
            advanceNodePtr( writePtr );            
        }

        deleteTrailingNodes( writePtr->nextPtr );
        lastPtr = writePtr;
        lastPtr->nextPtr = 0;
    }

    currentListLength = right.currentListLength;
    return right;
}

//===================================insert, delete================================================================
template< typename DATATYPE >
void List< DATATYPE >::insertAtFront( const DATATYPE& value ) 
{
    ListNode< DATATYPE >* newNodePtr = getNewNode( value );

    if ( isEmpty() ) 
        firstPtr = lastPtr = newNodePtr;

    else {
        newNodePtr->nextPtr = firstPtr;
        firstPtr = newNodePtr;
    }

    currentListLength++;
} 

template< typename DATATYPE >
void List< DATATYPE >::insertAtBack( const DATATYPE &value ) 
{
   ListNode< DATATYPE >* newNodePtr = getNewNode( value );

    if ( isEmpty() )  
        firstPtr = lastPtr = newNodePtr;
    else {  
        lastPtr->nextPtr = newNodePtr;
        lastPtr = newNodePtr;
    } 

    currentListLength++;
} 

template< typename DATATYPE >
bool List< DATATYPE >::deleteFromFront()
{
    if ( isEmpty() )  
        return false;  
    else 
    {  
        ListNode< DATATYPE >* toBeDeletedPtr = firstPtr;

        if ( getLength() == 1 )
            firstPtr = lastPtr = 0;
        else
            firstPtr = firstPtr->nextPtr;

        delete toBeDeletedPtr;
        currentListLength--;
        return true;  // delete successful
    } 
}

template< typename DATATYPE >
bool List< DATATYPE >::deleteFromBack()
{
    if ( isEmpty() )
        return false;
    else 
    {
        ListNode< DATATYPE > *toBeDeletedPtr = lastPtr;

        if ( getLength() == 1 )
            firstPtr = lastPtr = 0;
        else 
        {
            ListNode< DATATYPE >* traversalPtr = firstPtr;

            // locate second-to-last element
            while( traversalPtr->nextPtr != lastPtr )
                traversalPtr = traversalPtr->nextPtr;

            lastPtr = traversalPtr;
            lastPtr->nextPtr = 0;
        } 

        delete toBeDeletedPtr;
        currentListLength--;
        return true;  // delete successful
    }  
}

//=====================================read/read-remove functions=========================
template< typename DATATYPE >
bool List< DATATYPE >::readFromFront( DATATYPE &writeDestination ) const
{
    if ( isEmpty() )  
      return false;  
    else {  
      writeDestination = firstPtr->data;  // data being read, operator= required if data is object
      return true;  // read successful
   } 
}

template< typename DATATYPE >
bool List< DATATYPE >::readFromBack( DATATYPE &writeDestination ) const
{
    if ( isEmpty() )  
      return false;  
    else {  
      writeDestination = lastPtr->data;  // data being read, operator= required if data is object
      return true;  // read successful
   } 
}

template< typename DATATYPE >
bool List< DATATYPE >::removeFromFront( DATATYPE &writeDestination )
{
    return readFromFront( writeDestination ) ? deleteFromFront() : false;
}

template< typename DATATYPE >
bool List< DATATYPE >::removeFromBack( DATATYPE &writeDestination )
{
    return readFromBack( writeDestination ) ? deleteFromBack() : false;
} 

//================================================concatenate==============================================
template< typename DATATYPE >
void List< DATATYPE >::concatenate( List< DATATYPE >& trailingList )
{
    //missong length adjustment
    if( isEmpty() ) 
        firstPtr = trailingList.firstPtr;
    else 
        lastPtr->nextPtr = trailingList.firstPtr;

    lastPtr = trailingList.lastPtr;
    trailingList.firstPtr = trailingList.lastPtr = 0;
}


//=======================================status functions=================================
template< typename DATATYPE > 
bool List< DATATYPE >::isEmpty() const 
{ 
   return firstPtr == 0; 
}

template< typename DATATYPE >
void List< DATATYPE >::print() const
{
   if ( isEmpty() ) {
      cout << "The list is empty.\n";
      return;
   }

   ListNode< DATATYPE >* traversalPtr = firstPtr;

   while ( traversalPtr != 0 ) {
      cout << traversalPtr->data << ' ';// overloaded << required if data is user defined type
      traversalPtr = traversalPtr->nextPtr;
   } 

   cout << endl;
} 

template< typename DATATYPE >
int List< DATATYPE >::getLength() const 
{
    return currentListLength;
}

//===========================================util functions============================
template< typename DATATYPE >
ListNode< DATATYPE >* List< DATATYPE >::getNewNode( const DATATYPE& value ) 
{
    return new ListNode< DATATYPE >( value );// class ListNode constructor call
} 

template< typename DATATYPE >
void List< DATATYPE >::deleteTrailingNodes( ListNode< DATATYPE >* startingNodePtr )
{
    while( startingNodePtr != 0 )
    {
        ListNode< DATATYPE >* toBeDeletedPtr = startingNodePtr;
        advanceNodePtr( startingNodePtr );
        delete toBeDeletedPtr;
    }
}

template< typename DATATYPE >
void List< DATATYPE >::advanceNodePtr( ListNode< DATATYPE >*& nodePtr )
{
    if( nodePtr != 0 )
        nodePtr = nodePtr->nextPtr;
}

template< typename DATATYPE >
bool List< DATATYPE >::isThereNodeAhead( ListNode< DATATYPE >* nonNullNodePtr )
{
    return nonNullNodePtr->nextPtr == 0 ? false : true;
}

#endif
