
template<typename Head, typename... Node>
struct List {
    typedef Head head;
    typedef List<Node...> next;
    int size = (sizeof...(Node)) + 1;
};

template<typename Type, typename... TypeList>
struct PrependList {
    typedef List<Type, TypeList...> list;
};

// declaration of the general case
template<int N, typename TypeList>
struct GetAtIndex {};

// recursion for N > 0
template<int N, typename TypeHead, typename... Node>
struct GetAtIndex<N, List<TypeHead, Node...>> {
    typedef typename GetAtIndex<N - 1, List<Node...>>::TypeHead value;  
};

// Specialization for N = 0
// This is the base case for the recursion
template<typename TypeHead, typename... Node>
struct GetAtIndex<0, List<TypeHead, Node...>> {
    typedef TypeHead value;
};
