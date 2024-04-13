#ifndef LIST_H
#define LIST_H

/*************            List             ****************/
// declaration of the general case
template<typename... Nodes>
struct List {
    constexpr static int size = 0;
};

template<typename Head, typename... Nodes>
struct List<Head, Nodes...> {
    typedef Head head;
    typedef List<Nodes...> next;
    constexpr static int size = (sizeof...(Nodes)) + 1;
};

/*************            PrependList             ****************/
// declaration of the general case
template<typename Type, typename TypeList>
struct PrependList {};

template<typename Type, typename... TypeList>
struct PrependList<Type, List<TypeList...>> {
    typedef List<Type, TypeList...> list;
};

/*************            GetAtIndex             ****************/
// declaration of the general case
template<int N, typename TypeList>
struct GetAtIndex {};

// Specialization for N = 0
// This is the base case for the recursion
template<typename TypeHead, typename... TypeList>
struct GetAtIndex<0, List<TypeHead, TypeList...>> {
    typedef TypeHead value;
};

// recursion for N > 0
template<int N, typename TypeHead, typename... TypeList>
struct GetAtIndex<N, List<TypeHead, TypeList...>> {
    typedef typename GetAtIndex<N - 1, List<TypeList...>>::value value;  
};



/*************            SetAtIndex             ****************/
// declaration of the general case
template<int N, typename Type, typename TypeList>
struct SetAtIndex {};

// recursion for N > 0
template<int N ,typename Type, typename TypeHead, typename... TypeList>
struct SetAtIndex<N, Type, List<TypeHead, TypeList...>> {
    typedef typename PrependList<TypeHead, typename SetAtIndex<N - 1, Type, List<TypeList...>>::list>::list list;
};

// Specialization for N = 0
// Changing the value in the Nth node
template<typename Type, typename TypeHead, typename... TypeList>
struct SetAtIndex<0, Type, List<TypeHead, TypeList...>> {
    typedef List<Type, TypeList...> list;
};

#endif // LIST_H