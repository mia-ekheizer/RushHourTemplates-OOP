#ifndef UTILITIES_H
#define UTILITIES_H

//Conditional
template<bool expr, typename T, typename F>
struct Conditional {
    typedef T value;
};

template<typename T, typename F>
struct Conditional<false, T, F> {
    typedef F value;
};


//ConditionalInteger
template<bool expr, int T, int F>
struct ConditionalInteger {
    constexpr static int value = T;
};

template<int T, int F>
struct ConditionalInteger<false, T, F> {
    constexpr static int value = F;
};

#endif // UTILITIES_H