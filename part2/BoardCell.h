#ifndef BOARDCELL_H
#define BOARDCELL_H

#include "CellType.h"
#include "Direction.h"

template<CellType cell_type, Direction direction, int len_vehicle>
struct BoardCell {
    constexpr static CellType type = cell_type;
    constexpr static Direction direction = direction;
    constexpr static int length = len_vehicle;
};

#endif // BOARDCELL_H