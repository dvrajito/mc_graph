/***************************************************************************

FILENAME:       pq.h  ("pq" is short for "priority queue")

PROGRAMMERS:    W. Knight &  _________________________

DATE:           January 10, 1994 (the file was first created by W. Knight)
MODIFIED:       November 4, 1998 (changed some parameters and updated
                                    the descriptive terminology)
MODIFIED:       March 24, 1999   (changed the tests for empty; expanded
                                    and corrected the documentation)
MODIFIED:       March 18, 2000   (introduced a boolean data type;
                                    corrected some documentation errors)
MODIFIED:       January 20, 2002 (switched to built-in "bool" type)
MODIFIED:       November 3, 2002 (removed all "this->" prefixes;  reformatted)

LANGUAGE:       GNU C++

DESCRIPTION:    This file contains the documentation and definition of
                a template class named "priority-queue" that implements the
                ADT type of the same name.  Priority queues are described
                in almost all modern data structures texts.

IMPORTANT NOTE: The code for the member functions of the class "priority-queue"
                are included in this header file because the class is a
                template class, which makes it impossible under GNU C++
                to compile the code for the member functions separately
                from client code.

------------------------------------------------------------------------------

                         WHAT IS A PRIORITY QUEUE?

Briefly, a "priority queue" is a structure into which objects of some
specified type can be inserted.  Each object must have some associated
"priority value" that allows the objects to be partially ordered by
priority value.  It is permissible (indeed, expected) that distinct
objects may have the same priority value.  Once an object  x  has been
inserted into a priority queue, it cannot be removed until all objects
with higher priority in the structure have been removed, and
possibly even some objects with the same priority.  It is possible
that some objects with the same priority as  x  will be inserted
later than  x  and yet be removed sooner.

Note that a "priority queue" is not a "queue" in the usual FIFO sense of
that word.


        INSTRUCTIONS ON HOW TO USE THE MEMBER FUNCTIONS OF THIS CLASS

Clients of the "priority queue" class can use it to define a priority queue
of objects of any desired data type "otype" having the following properties.

  (1) The comparison operators  < , <= , > , and >=  must be defined on
      all objects  x  and  y  in the data type "otype" in such a way that
      x < y  if and only if  x  has lower priority than  y  (and similarly
      for the other operators).  In that sense, the "priority_queue" class
      implemented here is a max-queue.

       Note, however, that if you wish to use this class to create a priority
       min-queue using objects on which the comparison operators are already
       defined, you can do it as follows:  create a new object data type that
       just puts "wrapping paper" around objects of the original type, and then
       overload the comparison operators for the new type to be the reverse of
       the operators on the old type.

  (2) An assignment operator must be defined on objects of type "otype"
      so that appropriate copies of the objects of this type can be stored in
      these priority queues.  This assignment operator can be the default
      assignment operator provided by C++ (the one that performs bit-by-bit
      "shallow" copying) or an overloaded assignment operator, whichever is
      appropriate for copying the "otype" objects into the priority_queue.

  (3) Every object  x  of type "otype" must be "printable" in the sense that
      the "put to" operator  <<  is defined on the objects, so that an
      instruction such as  "cout << x << endl; "  will compile.


INSTANTIATION OF AN EMPTY PRIORITY QUEUE

Clients of the "priority_queue" class can use it to create an empty priority
queue of objects of any specified type.  For example, a client program can
define two priority queues named "pq1" and "pq2" as follows:

   priority_queue<long int> pq1, pq2;

Both priority queues will be empty when they first come into existence.
As another example, suppose a struct or class named "transaction" has
been defined previously.  Then the following statement will create an
empty priority queue that can hold objects of transaction type:

   priority_queue<transaction> transaction_pq;

Here it is important that the client have a constructor that requires no
arguments for the class "transaction".  That is, either there should be a
constructor that has an empty parameter list or else there should be a
constructor that has a default value provided for each of its parameters.

As a final example, suppose a struct or class named "customer_record"
has been defined previously and suppose many objects of that type have
been created and are being held in one or more other data structures.
It may be desirable to create a priority queue of pointers to some of these
customer_records.  Note, however, that pointers in C++ already have the
relational operators  < , <= , > , and >=  defined on them (the pointers
are compared as unsigned integers), and these operators have nothing to
do with the priority values of the customer records they point to.  If a
client programmer wishes to have a priority queue of pointers to customer
records, these cannot be "raw" pointers.  Instead, they must be pointers
that have been "packaged".  That is, the programmer can create a class
with a customer record pointer as its only data member, and then the
relational operators must be overloaded in the class so that they
dereference the pointer member to get at the actual customer record and
its priority value.


INSTANTIATING A PRIORITY QUEUE FROM AN ARRAY OF OBJECTS IN RANDOM ORDER

The priority_queue class has a second constructor function that
allows a priority queue to be created from an existing array  A  of objects
in random order.  This constructor must be passed the address of the
array  A  and the number of objects in the array.  The objects in  A  must be
stored in  A  starting at cell 0.  Thus, for example, the definition

priority_queue<transaction> pq(store, n);

will create a non-empty priority queue named pq (provided n is not zero).
The queue will initially contain a copy of each of the transactions in the
array "store".  This definition of pq will not affect the array "store".  The
objects in "store" will be copied into the priority queue pq.


INSERTING NEW OBJECTS INTO A PRIORITY QUEUE

A client program that has created a priority queue named "pq" can insert
an object  x  of the appropriate type into  pq  by using the overloaded
+= operator with the following syntax:

   pq += x;

Note that a COPY of  x  is being placed in the priority queue, not the object
itself.  If that is not what you as the programmer want, then you should
consider creating a priority queue of "packaged" pointers to the objects.

A priority queue is said to be "full" if no more objects can be inserted
into the priority queue because of memory limitations at the instant when
the insertion is attempted.  The expression  pq += x  (above) returns the
bool (logical) value false if  pq  is full at the moment when the expression
is evaluated.  This indicates that  x  was not inserted into  pq .  The
expression  pq += x  returns true if the insertion operation succeeds.
Thus it is usually wise for clients of the priority_queue class to place
such expressions inside conditional statements;  e.g.,

   if (!(pq += x))
   {
      cout << "Attempt to overflow priority queue" << endl;
      overflow_handler(x);
   }

where "overflow_handler" is a function designed to handle this unpleasant
possibility.


REMOVING AN OBJECT FROM A PRIORITY QUEUE

To perform the delete-max operation (i.e., to remove an object with highest
priority) from a priority queue, use the -- operator with this syntax:

   --pq

This expression returns a copy of a highest priority object and -- as a side
effect -- removes (in the logical sense) that object from the priority queue.
Clients will usually use this operation in connection with an assignment
statement:

   y = --pq;

It may happen that the object will be removed only in the logical sense.
That is, the object may continue to exist in the computer's memory until it
is overwritten or the priority queue  pq  goes out of scope.

IT IS A FATAL ERROR to call this operator function when pq is empty.
Thus the next function should be of considerable interest.


TESTING WHETHER A PRIORITY QUEUE IS EMPTY

To test whether a priority queue pq is empty, write

     if (!pq)
       .......

The operator ! has been overloaded for this purpose.


TESTING WHETHER A PRIORITY QUEUE IS NON-EMPTY

A cast operator has been defined so that an expression such as

   while (pq)  ...

will continue so long as pq is non-empty.


MAKING A PRIORITY QUEUE EMPTY

To make pq empty, call

     pq.make_empty();

which returns no value.  It does no harm to call this when pq is already empty.


INSPECTING AN OBJECT WITH HIGHEST PRIORITY IN A PRIORITY QUEUE

To obtain a copy of an object with highest priority in pq (i.e., the
object that will be removed the next time the operator -- is applied
to pq), write

     x = pq.highest();

which will place in  x  a copy of the object with highest priority in pq
without changing  pq  in any way.  IT IS A FATAL ERROR to call "highest"
when the queue is empty.


ASSIGNMENT AND COPYING OF PRIORITY QUEUES

No assignment operator has been provided for objects of "priority_queue"
type.  Thus only a shallow copy of a priority queue will be made if
an assignment is performed.

No copy constructor has been provided for objects of "priority_queue"
type.  Thus only a shallow copy of a priority queue will be made if
(for example) a priority queue is passed by value.


PRINTING THE CONTENTS OF A PRIORITY QUEUE

As stated above, the put-to operator << must be defined on the data type
of the objects placed in a priority_queue, so that an instruction such as

   cout << x << endl;

will compile.  This property is used only in the member function "print",
which can be used to display the entire contents of a priority queue  pq
as follows:

   pq.print();

This does not change  pq  in any way.  If  pq  is empty, a message
to that effect will be printed.  This print function was written primarily
for purposes of debugging the code for this class, but it has been left
in the class for the convenience of any programmer who wishes to modify the
code and test the modification.


****************************************************************************/

#include <cstdlib>
#include <iostream>
using namespace std;

/***************  C L A S S   P R I O R I T Y _ Q U E U E ******************/

template <class otype>   // "otype" is short for "object type"
class priority_queue
{
public:

    priority_queue(); // constructor function for an empty priority queue

    priority_queue(otype store[], int n); // constructor for a queue that
    // is initialized to contain copies of the objects
    // in the array store[0..n-1] .

    ~priority_queue();  // destructor function

    void make_empty();

    operator bool() const;     // Casts priority queue to false if empty.
    bool operator! () const;   // Casts priority queue to true if empty.

    bool operator+= (const otype& x);

    otype operator-- ();

    void dequeue(otype& x);

    otype highest() const;

    void print() const;

private:

    otype* heap;   // Array for storing the objects. Dynamically allocated.

    enum constant { DEFAULT_CAPACITY = 20 };

    int capacity;  // Current capacity of the heap (does NOT count cell 0
    // in the array).  There should always be capacity + 1 
    // cells in the array.

    int size;      // Current number of objects in the heap.
    // When size == capacity, the array is full.

    void percolate_up(int i);

    void sift_down(int i);

    bool enlarge();
};

/*********************  I M P L E M E N T A T I O N  *********************

The priority_queue class is implemented using a binary max-heap stored in
a dynamically allocated array named "heap".  Implementation of binary heaps
using arrays is well-described in texts that discuss the priority queue ADT,
so the details will not be described here.  An interesting feature of the
implementation is that the length of the array can change to accomodate
data that would otherwise cause it to overflow.

*****************************************************************************/


/*******************  F I R S T  C O N S T R U C T O R  *********************

This function creates an empty binary heap.  Documented and coded by
W. Knight.  */

template <class otype>
priority_queue<otype> ::priority_queue()
{
    size = 0;
    capacity = DEFAULT_CAPACITY;
    heap = new otype[capacity + 1];
    if (heap == NULL)
    {
        cout << "Memory allocation failure.  Program terminated." << endl;
        exit(1);
    }
}


/******************  S E C O N D   C O N S T R U C T O R  ********************

The "store" parameter is assumed to contain "n" objects of type otype
in cells  0  through  n-1 .  The objects are copied into the internal
array of the priority queue and then, using a well-known efficient
"heapification" algorithm, are rearranged into a binary heap.
Documented by W. Knight.  Coded by Dana Vrajitoru. */

template <class otype>
priority_queue<otype> ::priority_queue(otype store[], int n)
{
    heap = new otype[2 * n + 1];
    if (!heap)
    {
        cout << "Memory allocation failure.  Program terminated." << endl;
        exit(1);
    }
    size = n;
    capacity = 2 * n;

    int i;
    for (i = n; i > n / 2; --i)
        heap[i] = store[i - 1];

    for (i = n / 2; i >= 1; --i)
    {
        heap[i] = store[i - 1];
        sift_down(i);
    }
}


/***********************  D E S T R U C T O R  *******************************

Coded by W. Knight.   */

template <class otype>
priority_queue<otype> :: ~priority_queue()
{
    delete[] heap;
}


/**********************  M A K E   E M P T Y  *******************************

This "empties out" a priority queue.  Documented and coded by W. Knight.  */

template <class otype>
void priority_queue<otype> ::make_empty()
{
    delete[] heap;
    size = 0;
    capacity = DEFAULT_CAPACITY;
    heap = new otype[capacity + 1];
    if (heap == NULL)
    {
        cout << "Memory allocation failure.  Program terminated." << endl;
        exit(1);
    }
}


/*********************  C A S T   T O   B O O L E A N  ***********************

This function casts a priority queue to the bool value true if the priority
queue is non-empty, and to false if it is not.  Documented by W. Knight.
Coded by Dana Vrajitoru. */

template <class otype>
priority_queue<otype> :: operator bool() const
{
    return (size != 0);
}


/************************  O P E R A T O R  !   ******************************

When ! is placed in front of a priority queue variable, it returns the
boolean value true if the priority queue is empty, and false otherwise.
Documented by W. Knight.  Coded by Dana Vrajitoru.   */

template <class otype>
bool priority_queue<otype> :: operator! () const
{
    return (size == 0);
}


/*********************  P E R C O L A T E   U P  ***************************

This function causes the object that's initially in cell  i  of the binary
heap to "percolate" up to its appropriate position in the heap.
Documented by W. Knight.  Coded by Dana Vrajitoru.   */

template <class otype>
void priority_queue<otype> ::percolate_up(int i)
{
    heap[0] = heap[i];

    while (heap[0] > heap[i / 2])
    {
        heap[i] = heap[i / 2];
        i = i / 2;
    }

    heap[i] = heap[0];
}


/*****************************  S I F T   D O W N  ***************************

This function causes the object that's initially in cell  i  of the binary
heap to "sift down" to its appropriate position in the heap.  Documented
by W. Knight.  Coded by Dana Vrajitoru.   */

template <class otype>
void priority_queue<otype> ::sift_down(int i)
{
    int child = 2 * i;

    otype temp = heap[i];

    while (child <= size)
    {
        // If the two children are equal, choose the right one so that
        // you can get to the bottom of the tree faster.

        if (child < size && heap[child] <= heap[child + 1])
            ++child;

        if (temp < heap[child])
        {
            heap[i] = heap[child];
            i = child;
            child = 2 * i;
        }
        else
            child = size + 1;  // to stop the loop
    }

    heap[i] = temp;
}


/**************************  E N L A R G E  ********************************

This function attempts to dynamically allocate an array larger than the
one that currently holds the binary heap.  If it succeeds, it copies
the current binary heap into the larger array, deallocates the smaller array,
and then returns the boolean value true.  If it fails, it returns false.
The algorithm used is the following:  first an attempt is made to allocate
a new array twice as large as the old one;  if that fails, an attempt is
made to allocate a new array that's half again as large as the old;  if
that fails, we try for one that's a quarter again as large, and so on
down to an array that contains only one more cell than the original.
Documented and coded by W. Knight. */

template <class otype>
bool priority_queue<otype> ::enlarge()
{
    int new_cap = 2 * capacity;

    while (new_cap > capacity)
    {
        otype* temp = new otype[new_cap + 1];

        if (temp)
        {
            for (int i = 0; i <= size; ++i)
                temp[i] = heap[i];
            delete heap;
            heap = temp;
            capacity = new_cap;
            return true;        // RETURN an indication of success.
        }
        else
            new_cap = capacity + (new_cap - capacity) / 2;
    }

    // If the loop terminates because new_cap <= capacity, indicate failure.
    return false;
}


/************************  O P E R A T O R + =   ****************************

This function attempts to insert a copy of the object  x  into the priority
queue.  If it succeeds it returns true;  if not, it returns false.
Documented by W. Knight.  Coded by W. Knight and Dana Vrajitoru.  */

template <class otype>
bool priority_queue<otype> :: operator+= (const otype& x)
{
    if (size == capacity)  // The heap array is full; try to enlarge
        if (!enlarge())           // If enlargement failed
            return false;           // RETURN from the function

    heap[++(size)] = x;

    percolate_up(size);

    return true;
}


/*************************  O P E R A T O R - -   **************************

This function attempts to remove an object of highest priority from
the priority queue and return a copy of it as the value of the function.
If the priority queue is empty, this will produce a fatal error (the
entire program will be aborted).  Documented by W. Knight.
Coded by W. Knight and Dana Vrajitoru.*/

template <class otype>
otype priority_queue<otype> :: operator--()
{
    if (size == 0)    // if the priority queue is empty
    {
        cout << "Deletion attempted on an empty priority queue." << endl;
        cout << "Program terminated." << endl;
        exit(1);
    }

    else
    {
        otype temp = heap[1];  // Move highest priority item to safe location.

        if (size == 1)
            size = 0;
        else
        {
            heap[1] = heap[size--];  // Move last object to front of array.

            sift_down(1);
        }

        return temp;
    }
}


/*************************  D E Q U E U E   **************************

This function attempts to remove an object of highest priority from
the priority queue and store its value in the parameter. This is
written for the classes of base objects that require the assignment
operator to be defined with a reference parameter and cannot use the
operator --.  If the priority queue is empty, this will produce a
fatal error (the entire program will be aborted).  Documented by
D. Vrajitoru.  Coded by W. Knight and D. Vrajitoru.
*/

template <class otype>
void priority_queue<otype> ::dequeue(otype& x)
{
    if (size == 0)    // if the priority queue is empty
    {
        cout << "Deletion attempted on an empty priority queue." << endl;
        cout << "Program terminated." << endl;
        exit(1);
    }

    else
    {
        x = heap[1];  // Copy the highest priority item into the parameter.

        if (size == 1)
            size = 0;
        else
        {
            heap[1] = heap[size--];  // Move last object to front of array.

            sift_down(1);
        }
    }
}

/**************************  H I G H E S T  ****************************

This function attempts to return a copy of an item with highest priority
in the priority queue without changing the priority queue.  It is a fatal
error to call this function when the priority queue is empty.
Documented by W. Knight.  Coded by W. Knight and Dana Vrajitoru */

template <class otype>
otype priority_queue<otype> ::highest() const
{
    if (size == 0)    // if the priority queue is empty
    {
        cout << "Object access attempted on an empty priority queue." << endl;
        cout << "Program terminated." << endl;
        exit(1);
    }
    else
        return heap[1];

}


/**************************  P R I N T  ********************************

Assumes that each object  x  in the priority queue can be printed by a
command of the form "cout << x << endl;".   Documented by W. Knight.
Coded by W. Knight and Dana Vrajitoru. */

template <class otype>
void priority_queue<otype> ::print() const
{
    if (size == 0)
        cout << "The priority queue cannot be printed because it is empty.\n";

    else
        for (int i = 1; i <= size; ++i)
            cout << heap[i] << endl;
}

