/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmed <ahmed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:18:00 by mcombeau          #+#    #+#             */
/*   Updated: 2025/01/31 15:32:38 by ahmed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iterator>
#include <list>
#include <vector>

/* ---------------------------------------------------------- SHARED */

/**
 * @brief Default constructor - Creates empty PmergeMe object
 */
PmergeMe::PmergeMe( void ) {}

/**
 * @brief Destructor - Frees all dynamic memory
 */
PmergeMe::~PmergeMe( void ) {
	delete _unsortedVector;
	delete _sortedVector;
	delete _unsortedList;
	delete _sortedList;
}

/**
 * @brief Main constructor - Initializes with input array
 * @param array Input integers to sort
 * @param array_size Array length
 * @param container VECTOR or LIST
 */
PmergeMe::PmergeMe( int* array, int array_size, bool container ) : _container( container )
{
	_unsortedVector = new std::vector<int>();
	_sortedVector = new std::vector<int>();
	_unsortedList = new std::list<int>();
	_sortedList = new std::list<int>();
	if ( container == VECTOR )
	{
		_fillVectorFromArray( array, array_size );
	}
	else if ( container == LIST )
	{
		_fillListFromArray( array, array_size );
	}
}

/**
 * @brief Copy constructor - Creates a copy of the input PmergeMe object
 * @param src Input PmergeMe object to copy
 */
PmergeMe::PmergeMe( PmergeMe & src )
{
	( void )src;
}

/**
 * @brief Assignment operator - Assigns the input PmergeMe object to this object
 * @param src Input PmergeMe object to assign
 * @return This PmergeMe object
 */
PmergeMe & PmergeMe::operator=( PmergeMe & src )
{
	( void )src;
	return ( *this );
}

/**
 * @brief Main sorting function - Routes to vector or list implementation
 * 
 * Calls the appropriate sorting function based on container type.
 * Uses Ford-Johnson merge-insert sort algorithm for both implementations.
 */
void PmergeMe::sort( void )
{
	if ( _container == VECTOR )
	{
		_sortVector();
	}
	else if ( _container == LIST )
	{
		_sortList();
	}
}

/**
 * @brief Calculates Jacobsthal numbers for optimal insertion sequence
 * @param n Position in sequence
 * @return nth Jacobsthal number
 * 
 * Sequence: 0,1,1,3,5,11,21,43,...
 * Formula: J(n) = J(n-1) + 2*J(n-2)
 * Used to determine optimal insertion order for pending elements
 */
int PmergeMe::_getJacobstahlNumber( int n )
{
	if ( n == 0 )
	{
		return ( 0 );
	}
	if ( n == 1 )
	{
		return ( 1 );
	}
	return ( _getJacobstahlNumber( n - 1 ) + 2 * _getJacobstahlNumber( n - 2 ) );
}

/* ---------------------------------------------------------- VECTOR */

/**
 * @brief Gets reference to sorted vector result
 * @return Reference to sorted vector
 */
std::vector<int> & PmergeMe::getSortedVector( void )
{
	return (*_sortedVector);
}

/**
 * @brief Fills vector container from input array
 * @param array Source array
 * @param array_size Length of array
 */
void PmergeMe::_fillVectorFromArray( int * array, int array_size )
{
	for ( int i = 0; i < array_size; i++ )
	{
		_unsortedVector->push_back( array[i] );
	}
}

/**
 * @brief Main vector sorting implementation using Ford-Johnson algorithm
 * 
 * Algorithm steps:
 * 1. Check if already sorted
 * 2. Extract straggler if odd length
 * 3. Split into pairs and sort each pair
 * 4. Sort pairs by larger element
 * 5. Create main chain from smaller elements
 * 6. Insert larger elements using Jacobsthal sequence
 * 7. Insert straggler if exists
 */
void PmergeMe::_sortVector( void )
{
	_printVector( *_unsortedVector, "Unsorted", PURPLE );
	if ( _isVectorAlreadySorted() == true )
	{
		std::vector<int>::iterator it = _unsortedVector->begin();
		for ( ; it != _unsortedVector->end(); it++ )
		{
			_sortedVector->push_back( *it );
		}
		return ;
	}
	bool hasStraggler = _unsortedVector->size() % 2 != 0;
	if ( hasStraggler )
	{
		_extractStraggler( *_unsortedVector );
	}
	std::vector< std::pair<int, int> > splitVector = _splitIntoPairs( *_unsortedVector );
	_sortEachPair( splitVector );
	_sortPairsByLargestValue( splitVector );
	_createSortedSequence( splitVector );
	if ( hasStraggler )
	{
		_insertStraggler( *_sortedVector );
	}
}

/**
 * @brief Checks if vector is already in sorted order
 * @return true if sorted, false otherwise
 */
bool PmergeMe::_isVectorAlreadySorted( void )
{
	std::vector<int>::iterator it = _unsortedVector->begin();
	for ( ; it != _unsortedVector->end(); it++ )
	{
		std::vector<int>::iterator next = it + 1;
		if ( next != _unsortedVector->end() && *it > *next )
		{
			return ( false );
		}
	}
	return ( true );
}

/**
 * @brief Creates pairs from unsorted vector elements
 * @param unsortedVector Input vector to pair
 * @return Vector of pairs with elements sorted within each pair
 * 
 * Example:
 * Input: [3,1,4,2]
 * Output: [(1,3),(2,4)]
 */
std::vector< std::pair<int, int> > PmergeMe::_splitIntoPairs( std::vector<int> & unsortedVector )
{
	std::vector< std::pair<int, int> > splitVector;
	int first = 0;
	std::vector<int>::iterator it = unsortedVector.begin();
	for ( ; it != unsortedVector.end(); it++ )
	{
		if ( first == 0 )
		{
			first = *it;
		}
		else
		{
			splitVector.push_back( std::make_pair( first, *it ) );
			first = 0;
		}
	}
	_printVector( splitVector, "Split pair", YELLOW );
	return ( splitVector );
}

/**
 * @brief Sorts elements within each pair
 * @param splitVector Vector of pairs to sort
 * 
 * Ensures smaller element is first in each pair
 */
void PmergeMe::_sortEachPair( std::vector< std::pair<int, int> > & splitVector )
{
	std::vector< std::pair<int, int> >::iterator it = splitVector.begin();
	for ( ; it != splitVector.end(); it++ )
	{
		if( it->first > it->second )
		{
			int tmp = it->first;
			it->first = it->second;
			it->second = tmp;
		}
	}
	_printVector( splitVector, "Split pair", YELLOW );
}

/**
 * @brief Sorts pairs by their larger elements
 * @param splitVector Vector of pairs to sort
 * 
 * Used to create optimal insertion order for merge step
 */
void PmergeMe::_sortPairsByLargestValue( std::vector< std::pair<int, int> > & splitVector )
{
	int length = splitVector.size();
	_insertionSortPairs( splitVector, length - 1 );
	_printVector( splitVector, "Split pair", YELLOW );
}

/**
 * @brief Insertion sort for pairs
 * @param splitVector Vector of pairs to sort
 * @param n Current position to sort up to
 * 
 * Recursively sorts pairs based on their larger elements
 */
void PmergeMe::_insertionSortPairs( std::vector< std::pair<int, int> > & splitVector, int n )
{
	if ( n == 0 )
	{
		return ;
	}
	else
	{
		_insertionSortPairs( splitVector, n - 1 );
		std::vector< std::pair<int, int> >::iterator it = splitVector.begin();
		std::advance( it, n );
		_insertElement( splitVector, *it, n - 1 );
	}
}

/**
 * @brief Handles straggler element for odd-length sequences
 * @param unsortedVector Vector to extract straggler from
 * 
 * If sequence length is odd, removes and stores last element
 * for later insertion
 */
void PmergeMe::_extractStraggler( std::vector<int> & unsortedVector )
{
	_straggler = unsortedVector.back();
	unsortedVector.pop_back();
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Straggler detected... "
			<< "[" << _straggler << "]" << std::endl;
		_printVector( *_unsortedVector, "Unsorted", PURPLE );
	}
}

/**
 * @brief Inserts straggler into final position
 * @param sortedVector Sorted vector to insert straggler into
 * 
 * Uses binary search to find correct position for straggler
 */
void PmergeMe::_insertStraggler( std::vector<int> & sortedVector )
{
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Inserting straggler...";
	}
	_insertAtBisectedIndex( sortedVector, _straggler );
	if (VERBOSE)
	{
		std::cout <<  RESET << std::endl;
		_printVector( sortedVector, "Sorted", GREEN );
	}
}

/**
 * @brief Binary search implementation
 * @param vector Sorted vector to search in
 * @param x Element to find position for
 * @return Index where x should be inserted
 * 
 * Used to efficiently find insertion points
 */
int PmergeMe::_bisect( std::vector<int> vector, int x )
{
	int lo = 0;
	int hi = vector.size();

	while ( lo < hi )
	{
		int mid = ( lo + hi ) / 2;
		std::vector<int>::iterator it = vector.begin();
		std::advance(it, mid);
		if ( x  < *it )
		{
			hi = mid;
		}
		else
		{
			lo = mid + 1;
		}
	}
	return ( lo );
}

/**
 * @brief Creates main chain from sorted pairs
 * @param splitVector Sorted vector of pairs
 * 
 * Builds initial sorted sequence using smaller elements,
 * preparing for insertion of larger elements
 */
void PmergeMe::_createSortedSequence( std::vector< std::pair<int, int> > & splitVector )
{
	std::vector<int> pending;

	std::vector< std::pair<int, int> >::iterator it = splitVector.begin();
	for ( ; it != splitVector.end(); it++ )
	{
		_sortedVector->push_back( it->second );
		pending.push_back( it->first );
	}
	_printVector( *_sortedVector, "Sorted", GREEN );
	_printVector( pending, "Pending", CYAN );
	std::vector<int> indexSequence = _createIndexInsertSequence( pending );

	_printVector( indexSequence, "Index Seq", PURPLE );
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Inserting...";
	}
	std::vector<int>::iterator isit = indexSequence.begin();
	for (; isit != indexSequence.end(); isit++)
	{
		int numberToInsert = pending[*isit - 1];
		_insertAtBisectedIndex( *_sortedVector, numberToInsert );
	}
	if (VERBOSE)
	{
		std::cout << RESET << std::endl;
		_printVector( *_sortedVector, "Sorted", GREEN );
	}
}

std::vector<int> PmergeMe::_createIndexInsertSequence( std::vector<int> pending )
{
	bool lastWasJacobNumber = false;
	int pendingSize = pending.size();
	std::vector<int> indexSequence;

	indexSequence.push_back( 1 );
	if (pendingSize == 1)
	{
		return ( indexSequence );
	}
	std::vector<int> jacobSequence = _buildJacobstahlInsertionSequence(
	                                     pendingSize);
	_printVector( jacobSequence, "Jacobstahl", PURPLE );
	int i = 1;
	while ( i <= pendingSize )
	{
		if ( jacobSequence.size() != 0 && lastWasJacobNumber == false )
		{
			indexSequence.push_back( jacobSequence[0] );
			jacobSequence.erase( jacobSequence.begin() );
			lastWasJacobNumber = true;
			continue;
		}
		std::vector<int>::iterator iit = indexSequence.begin();
		for ( ; iit != indexSequence.end(); iit++ )
		{
			if ( *iit == i )
			{
				i++;
			}
		}
		indexSequence.push_back( i );
		lastWasJacobNumber = false;
		i++;
	}
	return (indexSequence);
}

void PmergeMe::_insertAtBisectedIndex( std::vector<int> & vector, int element )
{
	if (VERBOSE)
	{
		std::cout << "[" << element << "]";
	}
	int index = _bisect( vector, element );
	vector.insert( vector.begin() + index, element );
}

std::vector<int> PmergeMe::_buildJacobstahlInsertionSequence( int size )
{
	std::vector<int> jacobSequence;
	int jacobIndex = 3;
	while ( _getJacobstahlNumber( jacobIndex ) < size - 1 )
	{
		jacobSequence.push_back( _getJacobstahlNumber( jacobIndex ) );
		jacobIndex++;
	}
	return ( jacobSequence );
}

/**
 * @brief Prints vector contents
 * @param vector Vector to print
 * @param name Name of vector
 * @param color Color to print in
 * 
 * Used for debugging and visualization
 */
template <typename T>
void PmergeMe::_printVector( std::vector<T> & vector, std::string name,
                             std::string color )
{
	if (!VERBOSE)
	{
		return ;
	}
	std::vector<T> * vectorToPrint = new std::vector<T>( vector);
	printContainer( *vectorToPrint, name, color);
	delete vectorToPrint;
}

/* ---------------------------------------------------------- LIST */

/**
 * @brief Main list sorting implementation
 * 
 * Follows same algorithm as vector version:
 * 1. Check if sorted
 * 2. Handle odd length
 * 3. Split into pairs
 * 4. Sort pairs
 * 5. Create main chain
 * 6. Insert pending elements
 */
void PmergeMe::_sortList( void )
{
	_printList( *_unsortedList, "Unsorted", PURPLE );
	if ( _isListAlreadySorted() == true )
	{
		std::list<int>::iterator it = _unsortedList->begin();
		for ( ; it != _unsortedList->end(); it++ )
		{
			_sortedList->push_back( *it );
		}
		return ;
	}
	bool hasStraggler = _unsortedList->size() % 2 != 0;
	if ( hasStraggler )
	{
		_extractStraggler( *_unsortedList );
	}
	std::list< std::pair<int, int> > splitList = _splitIntoPairs( *_unsortedList );
	_sortEachPair( splitList );
	_sortPairsByLargestValue( splitList );
	_createSortedSequence( splitList );
	if ( hasStraggler )
	{
		_insertStraggler( *_sortedList );
	}
}

/**
 * @brief Checks if list is already in sorted order
 * @return true if sorted, false otherwise
 */
bool PmergeMe::_isListAlreadySorted( void )
{
	std::list<int>::iterator it = _unsortedList->begin();
	for ( ; it != _unsortedList->end(); it++ )
	{
		std::list<int>::iterator next = it;
		std::advance(next, 1);
		if ( next != _unsortedList->end() && *it > *next )
		{
			return ( false );
		}
	}
	return ( true );
}

/**
 * @brief Creates pairs from unsorted list elements
 * @param unsortedList Input list to pair
 * @return List of pairs with elements sorted within each pair
 * 
 * Example:
 * Input: [3,1,4,2]
 * Output: [(1,3),(2,4)]
 */
std::list< std::pair<int, int> > PmergeMe::_splitIntoPairs( std::list<int> & unsortedList )
{
	std::list< std::pair<int, int> > splitList;
	int first = 0;
	std::list<int>::iterator it = unsortedList.begin();
	for ( ; it != unsortedList.end(); it++ )
	{
		if ( first == 0 )
		{
			first = *it;
		}
		else
		{
			splitList.push_back( std::make_pair( first, *it ) );
			first = 0;
		}
	}
	_printList( splitList, "Split pair", YELLOW );
	return ( splitList );
}

/**
 * @brief Sorts elements within each pair
 * @param splitList List of pairs to sort
 * 
 * Ensures smaller element is first in each pair
 */
void PmergeMe::_sortEachPair( std::list< std::pair<int, int> > & splitList )
{
	std::list< std::pair<int, int> >::iterator it = splitList.begin();
	for ( ; it != splitList.end(); it++ )
	{
		if( it->first > it->second )
		{
			int tmp = it->first;
			it->first = it->second;
			it->second = tmp;
		}
	}
	_printList( splitList, "Split pair", YELLOW );
}

/**
 * @brief Sorts pairs by their larger elements
 * @param splitList List of pairs to sort
 * 
 * Used to create optimal insertion order for merge step
 */
void PmergeMe::_sortPairsByLargestValue( std::list< std::pair<int, int> > & splitList )
{
	int length = splitList.size();
	_insertionSortPairs( splitList, length - 1 );
	_printList( splitList, "Split pair", YELLOW );
}

/**
 * @brief Insertion sort for pairs
 * @param splitList List of pairs to sort
 * @param n Current position to sort up to
 * 
 * Recursively sorts pairs based on their larger elements
 */
void PmergeMe::_insertionSortPairs( std::list< std::pair<int, int> > & splitList, int n )
{
	if ( n == 0 )
	{
		return ;
	}
	else
	{
		_insertionSortPairs( splitList, n - 1 );
		std::list< std::pair<int, int> >::iterator it = splitList.begin();
		std::advance( it, n );
		_insertElement( splitList, *it, n - 1 );
	}
}

/**
 * @brief Handles straggler element for odd-length sequences
 * @param unsortedList List to extract straggler from
 * 
 * If sequence length is odd, removes and stores last element
 * for later insertion
 */
void PmergeMe::_extractStraggler( std::list<int> & unsortedList )
{
	_straggler = unsortedList.back();
	unsortedList.pop_back();
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Straggler detected... "
			<< "[" << _straggler << "]" << std::endl;
		_printList( *_unsortedList, "Unsorted", PURPLE );
	}
}

/**
 * @brief Inserts straggler into final position
 * @param sortedList Sorted list to insert straggler into
 * 
 * Uses binary search to find correct position for straggler
 */
void PmergeMe::_insertStraggler( std::list<int> & sortedList )
{
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Inserting straggler...";
	}
	_insertAtBisectedIndex( sortedList, _straggler );
	if (VERBOSE)
	{
		std::cout <<  RESET << std::endl;
		_printList( sortedList, "Sorted ", GREEN );
	}
}

/**
 * @brief Creates main chain from sorted pairs
 * @param splitList Sorted list of pairs
 * 
 * Builds initial sorted sequence using smaller elements,
 * preparing for insertion of larger elements
 */
void PmergeMe::_createSortedSequence( std::list< std::pair<int, int> > & splitList )
{
	std::list<int> pending;

	std::list< std::pair<int, int> >::iterator it = splitList.begin();
	for ( ; it != splitList.end(); it++ )
	{
		_sortedList->push_back( it->second );
		pending.push_back( it->first );
	}
	_printList( *_sortedList, "Sorted", GREEN );
	_printList( pending, "Pending", CYAN );
	std::list<int> indexSequence = _createIndexInsertSequence( pending );

	_printList( indexSequence, "Index Seq", PURPLE );
	if (VERBOSE)
	{
		std::cout << CYAN << std::setw( 35 ) << std::left << "Inserting...";
	}
	std::list<int>::iterator isit = indexSequence.begin();
	for (; isit != indexSequence.end(); isit++)
	{
		int index = *isit;
		std::list<int>::iterator pit = pending.begin();
		std::advance(pit, index - 1);
		int numberToInsert = *pit;
		_insertAtBisectedIndex( *_sortedList, numberToInsert );
	}
	if (VERBOSE)
	{
		std::cout << RESET << std::endl;
		_printList( *_sortedList, "Sorted ", GREEN );
	}
}

std::list<int> PmergeMe::_createIndexInsertSequence( std::list<int> pending )
{
	bool lastWasJacobNumber = false;
	int pendingSize = pending.size();
	std::list<int> indexSequence;

	indexSequence.push_back( 1 );
	if (pendingSize == 1)
	{
		return ( indexSequence );
	}
	std::list<int> jacobSequence = _buildJacobstahlInsertionSequence(
	                                     pending );
	_printList( jacobSequence, "Jacobstahl", PURPLE );
	int i = 1;
	while (i <= pendingSize)
	{
		if ( jacobSequence.size() != 0 && lastWasJacobNumber == false )
		{
			indexSequence.push_back( *jacobSequence.begin() );
			jacobSequence.erase( jacobSequence.begin() );
			lastWasJacobNumber = true;
			continue;
		}
		std::list<int>::iterator iit = indexSequence.begin();
		for ( ; iit != indexSequence.end(); iit++ )
		{
			if ( *iit == i )
			{
				i++;
			}
		}
		indexSequence.push_back( i );
		lastWasJacobNumber = false;
		i++;
	}
	return (indexSequence);
}

/**
 * @brief Inserts element at bisected index
 * @param list List to insert into
 * @param element Element to insert
 * 
 * Uses binary search to find correct position for element
 */
void PmergeMe::_insertAtBisectedIndex( std::list<int> & list, int element )
{
	if (VERBOSE)
	{
		std::cout << "[" << element << "]";
	}
	int pos = _bisect( list, element );
	std::list<int>::iterator it = list.begin();
	std::advance(it, pos);
	list.insert( it, element );
}

/**
 * @brief Binary search implementation
 * @param list Sorted list to search in
 * @param x Element to find position for
 * @return Index where x should be inserted
 * 
 * Used to efficiently find insertion points
 */
int PmergeMe::_bisect( std::list<int> list, int x )
{
	int lo = 0;
	int hi = list.size();

	while ( lo < hi )
	{
		int mid = ( lo + hi ) / 2;
		std::list<int>::iterator it = list.begin();
		std::advance(it, mid);
		if ( x  < *it )
		{
			hi = mid;
		}
		else
		{
			lo = mid + 1;
		}
	}
	return ( lo );
}

/**
 * @brief Prints list contents
 * @param list List to print
 * @param name Name of list
 * @param color Color to print in
 * 
 * Used for debugging and visualization
 */
template <typename T>
void PmergeMe::_printList( std::list<T> & list, std::string name,
                             std::string color )
{
	if (!VERBOSE)
	{
		return ;
	}
	std::list<T> * listToPrint = new std::list<T>( list );
	printContainer( *listToPrint, name, color);
	delete listToPrint;
}

std::list<int> & PmergeMe::getSortedList( void )
{
    return (*_sortedList);
}

void PmergeMe::_fillListFromArray( int * array, int array_size )
{
    for ( int i = 0; i < array_size; i++ )
    {
        _unsortedList->push_back( array[i] );
    }
}

void PmergeMe::_insertElement( std::vector< std::pair<int, int> > & splitVector,
                               std::pair<int, int> element, int n )
{
    if ( n < 0 )
    {
        splitVector[0] = element;
    }
    else if ( element.second >= splitVector[n].second )
    {
        int size = splitVector.size();
        if ( n == size - 1 )
        {
            splitVector.push_back( element );
        }
        else
        {
            splitVector[n + 1] = element;
        }
    }
    else
    {
        int size = splitVector.size();
        if ( n == size - 1 )
        {
            splitVector.push_back( splitVector[n] );
        }
        else
        {
            splitVector[n + 1] = splitVector[n];
            _insertElement( splitVector, element, n - 1 );
        }
    }
}

void PmergeMe::_insertElement( std::list< std::pair<int, int> > & splitList,
                               std::pair<int, int> element, int n )
{
    std::list< std::pair<int, int> >::iterator it = splitList.begin();
    std::advance( it, n );
    std::list< std::pair<int, int> >::iterator next = it;
    next++;

    if ( n < 0 )
    {
        *splitList.begin() = element;
    }
    else if ( element.second >= it->second )
    {
        int size = splitList.size();
        if ( n == size - 1 )
        {
            splitList.push_back( element );
        }
        else
        {
            *next = element;
        }
    }
    else
    {
        int size = splitList.size();
        if ( n == size - 1 )
        {
            splitList.push_back( *it );
        }
        else
        {
            *next = *it;
            _insertElement( splitList, element, n - 1 );
        }
    }
}

std::list<int> PmergeMe::_buildJacobstahlInsertionSequence( std::list<int> pending )
{
    std::list<int> jacobSequence;
    int size = pending.size();
    int jacobIndex = 3;
    while ( _getJacobstahlNumber( jacobIndex ) < size - 1 )
    {
        jacobSequence.push_back( _getJacobstahlNumber( jacobIndex ) );
        jacobIndex++;
    }
    return ( jacobSequence );
}
