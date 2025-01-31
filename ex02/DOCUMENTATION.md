# PmergeMe - Function Documentation

## Class Overview
`PmergeMe` is a template class that implements the Ford-Johnson (merge-insert) sorting algorithm. It can work with different container types (std::vector, std::deque) to sort positive integers.

## Core Functions

### Constructor and Destructor
```cpp
template<typename Container>
PmergeMe();
~PmergeMe();
```
- **Purpose**: Initializes/cleans up the sorting object
- **Parameters**: None
- **Returns**: None

### Main Sorting Function
```cpp
void sort();
```
- **Purpose**: Main entry point for sorting the sequence
- **Process**:
  1. Checks if sequence needs sorting
  2. Records start time
  3. Performs Ford-Johnson sort
  4. Records end time
- **Returns**: None

## Helper Functions

### 1. makePairs
```cpp
void makePairs(std::vector<std::pair<typename Container::value_type, 
                                   typename Container::value_type> >& pairs);
```
- **Purpose**: Creates pairs from the input sequence
- **Process**:
  1. Takes input sequence
  2. Groups elements into pairs
  3. Handles odd element (straggler) if present
- **Parameters**: Vector to store the pairs
- **Example**:
  ```
  Input: [3,1,4,2]
  Output pairs: [(3,1), (4,2)]
  ```

### 2. insertionSort
```cpp
void insertionSort(Container& mainChain, const Container& pending);
```
- **Purpose**: Inserts elements from pending into mainChain
- **Process**:
  1. Uses binary search to find insertion point
  2. Inserts elements in Jacobsthal sequence order
- **Parameters**:
  - mainChain: The sorted sequence being built
  - pending: Elements waiting to be inserted
- **Example**:
  ```
  mainChain: [1,3]
  pending: [2,4]
  Result: [1,2,3,4]
  ```

### 3. jacobsthal
```cpp
int jacobsthal(int n);
```
- **Purpose**: Calculates nth Jacobsthal number
- **Formula**: J(n) = J(n-1) + 2×J(n-2)
- **Base Cases**:
  - J(0) = 0
  - J(1) = 1
- **Example**:
  ```
  n=0 → 0
  n=1 → 1
  n=2 → 1
  n=3 → 3
  n=4 → 5
  ```

### 4. generateJacobsthalSequence
```cpp
std::vector<int> generateJacobsthalSequence(int n);
```
- **Purpose**: Generates Jacobsthal sequence up to n
- **Process**:
  1. Starts with 1
  2. Generates numbers until exceeding n
- **Parameters**: n - upper limit
- **Returns**: Vector of Jacobsthal numbers
- **Example**:
  ```
  n=7 → [1,3,5]
  n=12 → [1,3,5,11]
  ```

### 5. binarySearch
```cpp
typename Container::iterator binarySearch(typename Container::value_type value,
                                       typename Container::iterator begin,
                                       typename Container::iterator end);
```
- **Purpose**: Finds insertion point for a value
- **Process**: Binary search implementation
- **Parameters**:
  - value: Element to insert
  - begin/end: Range to search in
- **Returns**: Iterator to insertion point
- **Complexity**: O(log n)

## Utility Functions

### 1. setSequence
```cpp
void setSequence(const Container& input);
```
- **Purpose**: Sets sequence to be sorted
- **Parameters**: Input container
- **Usage**: Before calling sort()

### 2. getSequence
```cpp
const Container& getSequence() const;
```
- **Purpose**: Returns current sequence
- **Returns**: Reference to sorted sequence
- **Usage**: After sorting is complete

### 3. getExecutionTime
```cpp
double getExecutionTime() const;
```
- **Purpose**: Returns sorting time
- **Returns**: Time in microseconds
- **Usage**: After sorting is complete

## Error Handling

### Error Class
```cpp
class Error : public std::exception
```
- **Purpose**: Custom exception handling
- **Methods**:
  - what(): Returns error message
- **Usage**: Thrown for invalid inputs

## Usage Example
```cpp
try {
    // Create sorter
    PmergeMe<std::vector<int> > sorter;
    
    // Set sequence
    std::vector<int> input = {5,2,8,1,9};
    sorter.setSequence(input);
    
    // Sort
    sorter.sort();
    
    // Get result
    const std::vector<int>& result = sorter.getSequence();
    
    // Get timing
    double time = sorter.getExecutionTime();
} catch (const PmergeMe::Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```
