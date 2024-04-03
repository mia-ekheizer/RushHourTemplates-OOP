
template<typename Head, typename... Node>
struct List {
    typedef Head head;
    typedef List<Node...> next;
    int size = (sizeof...(Node)) + 1;
};

template<typename, typename>
struct PrependList {};

template<typename Type, typename... TypeList>
struct PrependList<Type, List<TypeList...>> {
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

// declaration of the general case
template<int N, typename Type, typename... TypeList>
struct SetAtIndex {};

// recursion for N > 0
template<int N ,typename Type, typename TypeHead, typename... Node>
struct SetAtIndex<N, Type, List<TypeHead, Node...>> {
    typedef typename SetAtIndex<N - 1, Type, List<Node...>>::TypeHead list;
};

// Specialization for N = 0
// Changing the value in the Nth node
template<typename Type, typename TypeHead, typename... Node>
struct SetAtIndex<0, Type, List<TypeHead, Node...>> {
    typedef typename PrependList<Type, List<Node...>>::TypeHead list;
};