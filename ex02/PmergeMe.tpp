#pragma once

template<typename Container>
void PmergeMe<Container>::sort() {
    if (sequence.empty() || sequence.size() == 1)
        return;

    clock_t start = clock();

    // Step 1: Create and sort pairs
    std::vector<std::pair<typename Container::value_type, 
                         typename Container::value_type> > pairs;
    makePairs(pairs);

    // Step 2: Create main chain and pending elements
    Container mainChain;
    Container pending;

    // Sort pairs by first element
    std::sort(pairs.begin(), pairs.end());

    // Create initial sorted sequence and pending elements
    for (typename std::vector<std::pair<typename Container::value_type, 
         typename Container::value_type> >::const_iterator it = pairs.begin();
         it != pairs.end(); ++it) {
        mainChain.push_back(it->second);  // smaller elements
        pending.push_back(it->first);     // larger elements
    }

    // Handle odd element if exists
    if (sequence.size() % 2 == 1) {
        typename Container::value_type lastElement = sequence.back();
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), lastElement);
        mainChain.insert(pos, lastElement);
    }

    // Step 3: Insert remaining elements using Ford-Johnson sequence
    insertionSort(mainChain, pending);

    // Update sequence and execution time
    sequence = mainChain;
    executionTime = (static_cast<double>(clock() - start) / CLOCKS_PER_SEC) * 1000000;
}

template<typename Container>
void PmergeMe<Container>::makePairs(std::vector<std::pair<typename Container::value_type, 
                                                         typename Container::value_type> >& pairs) {
    for (size_t i = 0; i < sequence.size() - 1; i += 2) {
        typename Container::value_type first = sequence[i];
        typename Container::value_type second = sequence[i + 1];
        if (first < second)
            std::swap(first, second);
        pairs.push_back(std::make_pair(first, second));
    }
}

template<typename Container>
void PmergeMe<Container>::insertionSort(Container& mainChain, const Container& pending) {
    if (pending.empty())
        return;

    std::vector<int> jacobSequence = generateJacobsthalSequence(pending.size());
    std::vector<bool> inserted(pending.size(), false);

    // Insert elements according to Jacobsthal sequence
    for (size_t i = 0; i < jacobSequence.size(); ++i) {
        int idx = jacobSequence[i] - 1;
        if (idx >= static_cast<int>(pending.size()))
            continue;

        if (!inserted[idx]) {
            typename Container::iterator pos = binarySearch(pending[idx], mainChain.begin(), mainChain.end());
            mainChain.insert(pos, pending[idx]);
            inserted[idx] = true;
        }

        // Insert all remaining elements up to this position
        for (int j = idx - 1; j >= 0; --j) {
            if (!inserted[j]) {
                typename Container::iterator pos = binarySearch(pending[j], mainChain.begin(), mainChain.end());
                mainChain.insert(pos, pending[j]);
                inserted[j] = true;
            }
        }
    }
}

template<typename Container>
int PmergeMe<Container>::jacobsthal(int n) const {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return jacobsthal(n - 1) + 2 * jacobsthal(n - 2);
}

template<typename Container>
std::vector<int> PmergeMe<Container>::generateJacobsthalSequence(int n) const {
    std::vector<int> sequence;
    sequence.push_back(1);
    
    int i = 1;
    while (true) {
        int next = jacobsthal(i + 1);
        if (next > n)
            break;
        sequence.push_back(next);
        i++;
    }
    return sequence;
}

template<typename Container>
typename Container::iterator PmergeMe<Container>::binarySearch(
    typename Container::value_type value,
    typename Container::iterator begin,
    typename Container::iterator end) {
    return std::lower_bound(begin, end, value);
}

template<typename Container>
void PmergeMe<Container>::printSequence(const std::string& prefix) const {
    std::cout << prefix;
    for (typename Container::const_iterator it = sequence.begin(); 
         it != sequence.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
