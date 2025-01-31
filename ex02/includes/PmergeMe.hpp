/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:16:16 by mcombeau          #+#    #+#             */
/*   Updated: 2023/05/18 15:16:16 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLEASE_MERGE_ME_HPP
#define PLEASE_MERGE_ME_HPP

#include "utils.hpp"

#define VECTOR 0
#define LIST 1

/**
 * @class PmergeMe
 * @brief Implements the Ford-Johnson (merge-insert) sorting algorithm
 * 
 * This class provides functionality to sort integers using both std::vector
 * and std::list containers. It implements the Ford-Johnson algorithm which
 * combines merge sort and insertion sort techniques.
 */
class PmergeMe
{
	private:
		/* SHARED */
		bool _container;  // Flag to indicate which container type is being used (VECTOR or LIST)
		int _straggler;   // Stores the odd element when sequence length is odd

		/**
		 * @brief Default constructor (private to prevent direct instantiation)
		 */
		PmergeMe( void );

		/**
		 * @brief Calculates the nth Jacobsthal number
		 * @param n The position in the Jacobsthal sequence
		 * @return The nth Jacobsthal number
		 * 
		 * Jacobsthal numbers are used to determine the optimal insertion order
		 * Formula: J(n) = J(n-1) + 2*J(n-2)
		 */
		int _getJacobstahlNumber( int n );

		/* VECTOR OPERATIONS */
		std::vector<int> * _unsortedVector;  // Stores the original unsorted sequence
		std::vector<int> * _sortedVector;    // Stores the final sorted sequence

		/**
		 * @brief Fills vector with input array elements
		 * @param array Source array
		 * @param array_size Size of the array
		 */
		void _fillVectorFromArray( int * array, int array_size );

		/**
		 * @brief Main vector sorting function implementing Ford-Johnson algorithm
		 */
		void _sortVector( void );

		/**
		 * @brief Checks if vector is already sorted
		 * @return true if sorted, false otherwise
		 */
		bool _isVectorAlreadySorted( void );

		/**
		 * @brief Creates pairs from unsorted vector
		 * @param unsortedVector Vector to split into pairs
		 * @return Vector of pairs
		 * 
		 * Groups elements into pairs for the first step of Ford-Johnson sort
		 */
		std::vector< std::pair<int, int> > _splitIntoPairs( std::vector<int> & unsortedVector );

		/**
		 * @brief Sorts elements within each pair
		 * @param splitVector Vector of pairs to sort
		 * 
		 * Ensures smaller element is first in each pair
		 */
		void _sortEachPair( std::vector< std::pair<int, int> > & splitVector );

		/**
		 * @brief Sorts pairs by their larger elements
		 * @param splitVector Vector of pairs to sort
		 */
		void _sortPairsByLargestValue( std::vector< std::pair<int, int> > & splitVector );

		/**
		 * @brief Performs insertion sort on pairs
		 * @param splitVector Vector of pairs to sort
		 * @param n Number of elements to sort
		 */
		void _insertionSortPairs( std::vector< std::pair<int, int> > & splitVector, int n );

		/**
		 * @brief Inserts a pair at correct position
		 * @param splitVector Vector to insert into
		 * @param element Pair to insert
		 * @param n Current size of sorted portion
		 */
		void _insertElement( std::vector< std::pair<int, int> > & splitVector,
		                     std::pair<int, int> element, int n );

		/**
		 * @brief Creates the main chain and pending elements
		 * @param splitVector Sorted pairs to process
		 */
		void _createSortedSequence( std::vector< std::pair<int, int> > & splitVector );

		/**
		 * @brief Generates sequence for inserting pending elements
		 * @param pending Vector of pending elements
		 * @return Vector of insertion positions
		 */
		std::vector<int> _createIndexInsertSequence( std::vector<int> pending );

		/**
		 * @brief Builds Jacobsthal sequence for insertion order
		 * @param size Maximum size needed
		 * @return Vector of Jacobsthal numbers
		 */
		std::vector<int> _buildJacobstahlInsertionSequence( int size );

		/**
		 * @brief Inserts element using binary search
		 * @param vector Vector to insert into
		 * @param element Element to insert
		 */
		void _insertAtBisectedIndex( std::vector<int> & vector, int element );

		/**
		 * @brief Performs binary search
		 * @param vector Vector to search in
		 * @param x Element to find position for
		 * @return Insertion position
		 */
		int _bisect( std::vector<int> vector, int x );

		/**
		 * @brief Extracts odd element if exists
		 * @param unsortedVector Vector to extract from
		 */
		void _extractStraggler( std::vector<int> & unsortedVector );

		/**
		 * @brief Inserts straggler into final sequence
		 * @param sortedVector Vector to insert into
		 */
		void _insertStraggler( std::vector<int> & sortedVector );

		/* LIST OPERATIONS */
		std::list<int> * _unsortedList;  // Stores the original unsorted sequence
		std::list<int> * _sortedList;    // Stores the final sorted sequence

		/**
		 * @brief Fills list with input array elements
		 * @param array Source array
		 * @param array_size Size of the array
		 */
		void _fillListFromArray( int * array, int array_size );

		/**
		 * @brief Main list sorting function implementing Ford-Johnson algorithm
		 */
		void _sortList( void );

		/**
		 * @brief Checks if list is already sorted
		 * @return true if sorted, false otherwise
		 */
		bool _isListAlreadySorted( void );

		/**
		 * @brief Creates pairs from unsorted list
		 * @param unsortedList List to split into pairs
		 * @return List of pairs
		 * 
		 * Groups elements into pairs for the first step of Ford-Johnson sort
		 */
		std::list< std::pair<int, int> > _splitIntoPairs( std::list<int> & unsortedList );

		/**
		 * @brief Sorts elements within each pair
		 * @param splitList List of pairs to sort
		 * 
		 * Ensures smaller element is first in each pair
		 */
		void _sortEachPair( std::list< std::pair<int, int> > & splitList );

		/**
		 * @brief Sorts pairs by their larger elements
		 * @param splitList List of pairs to sort
		 */
		void _sortPairsByLargestValue( std::list< std::pair<int, int> > & splitList );

		/**
		 * @brief Performs insertion sort on pairs
		 * @param splitList List of pairs to sort
		 * @param n Number of elements to sort
		 */
		void _insertionSortPairs( std::list< std::pair<int, int> > & splitList, int n );

		/**
		 * @brief Inserts a pair at correct position
		 * @param splitList List to insert into
		 * @param element Pair to insert
		 * @param n Current size of sorted portion
		 */
		void _insertElement( std::list< std::pair<int, int> > & splitList,
		                     std::pair<int, int> element, int n );

		/**
		 * @brief Creates the main chain and pending elements
		 * @param splitList Sorted pairs to process
		 */
		void _createSortedSequence( std::list< std::pair<int, int> > & splitList );

		/**
		 * @brief Generates sequence for inserting pending elements
		 * @param pending List of pending elements
		 * @return List of insertion positions
		 */
		std::list<int> _createIndexInsertSequence( std::list<int> pending );

		/**
		 * @brief Builds Jacobsthal sequence for insertion order
		 * @param pending List of pending elements
		 * @return List of Jacobsthal numbers
		 */
		std::list<int> _buildJacobstahlInsertionSequence( std::list<int> pending );

		/**
		 * @brief Inserts element using binary search
		 * @param list List to insert into
		 * @param element Element to insert
		 */
		void _insertAtBisectedIndex( std::list<int> & list, int element );

		/**
		 * @brief Performs binary search
		 * @param list List to search in
		 * @param x Element to find position for
		 * @return Insertion position
		 */
		int _bisect( std::list<int> list, int x );

		/**
		 * @brief Extracts odd element if exists
		 * @param unsortedList List to extract from
		 */
		void _extractStraggler( std::list<int> & unsortedList );

		/**
		 * @brief Inserts straggler into final sequence
		 * @param sortedList List to insert into
		 */
		void _insertStraggler( std::list<int> & sortedList );

		template <typename T>
		void _printVector( std::vector<T> & vector, std::string name,
		                   std::string color );

		std::string _getVectorContentsAsString( std::vector<int> & vector );
		std::string _getVectorContentsAsString( std::vector< std::pair<int, int> > &
		                                        vector );

		template <typename T>
		void _printList( std::list<T> & list, std::string name,
		                   std::string color );

		std::string _getListContentsAsString( std::list<int> & list );
		std::string _getListContentsAsString( std::list< std::pair<int, int> > &
		                                        list );

	public:
		/**
		 * @brief Constructor
		 * @param array Input array to sort
		 * @param array_size Size of input array
		 * @param container Container type (VECTOR or LIST)
		 */
		PmergeMe( int* array, int array_size, bool container );

		/**
		 * @brief Copy constructor
		 * @param src Source object to copy
		 */
		PmergeMe( PmergeMe & src );

		/**
		 * @brief Destructor
		 */
		~PmergeMe( void );

		/**
		 * @brief Assignment operator
		 * @param src Source object to assign from
		 * @return Reference to this object
		 */
		PmergeMe & operator=( PmergeMe & src );

		/**
		 * @brief Main sorting function
		 * Sorts the sequence using Ford-Johnson algorithm
		 */
		void sort( void );

		/**
		 * @brief Gets sorted vector
		 * @return Reference to sorted vector
		 */
		std::vector<int> & getSortedVector( void );

		/**
		 * @brief Gets sorted list
		 * @return Reference to sorted list
		 */
		std::list<int> & getSortedList( void );
};

#endif
