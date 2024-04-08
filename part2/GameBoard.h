#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "List.h"

template<typename>
struct GameBoard {};

template<typename... TypeList>
struct GameBoard<List<TypeList...>> {
    typedef List<TypeList...> board;
    constexpr static int width = board::head::size;
    constexpr static int length = board::size;
};

#endif // GAMEBOARD_H