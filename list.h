#ifndef LIST_H
#define LIST_H

#include <iostream>
using std::cout;
using std::endl;

#include "listnode.h"

template< typename DATATYPE >
class List {

public:
    List();  
    ~List(); 
    List( List< DATATYPE >& listToCopy );
    const List< DATATYPE >& operator=( const List< DATATYPE >& right );
    
    void insertAtFront( const DATATYPE& value );
    void insertAtBack( const DATATYPE& value );
    bool deleteFromFront();
    bool deleteFromBack();

    // read only functions
    bool readFromFront( DATATYPE& writeDestination ) const;
    bool readFromBack( DATATYPE& writeDestination ) const;
    // these functions read data and delete it from list
    bool removeFromFront( DATATYPE& writeDestination );
    bool removeFromBack( DATATYPE& writeDestination );
    
    // trailing list is emptied
    void concatenate( List< DATATYPE >& trailingList );

    //status functions
    bool isEmpty() const;
    void print() const;
    int getLength() const;

private:
    ListNode< DATATYPE >* firstPtr;  
    ListNode< DATATYPE >* lastPtr;   
    int currentListLength;

    ListNode< DATATYPE >* getNewNode( const DATATYPE& );
    void constructCopyOfList( ListNode< DATATYPE >* startNodePtr, List< DATATYPE >& blankListRef );
    void deleteTrailingNodes( ListNode< DATATYPE >* startNodePtr );
    
    // takes reference to a pointer and advances its value to next node's address
    void advanceNodePtr( ListNode< DATATYPE >*& nodePtr );
    bool isThereNodeAhead( ListNode< DATATYPE >* nonNullNodePtr );
};

#include "list.cpp"
#endif
