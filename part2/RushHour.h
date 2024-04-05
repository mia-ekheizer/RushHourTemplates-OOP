#ifndef RUSHHOUR_H
#define RUSHHOUR_H

#include "MoveVehicle.h"
#include "GameBoard.h"

template <typename RowList, int Col, int Counter>
struct CheckWinHelper {
    constexpr static bool tmp = CheckWinHelper<RowList, Col + 1, Counter - 1>::result;
    constexpr static bool result = Conditional<GetAtIndex<Col, RowList>::value == EMPTY, tmp, false>::value;
};

template <typename RowList, int Col>
struct CheckWinHelper<RowList, Col, 0> {
    constexpr static bool result = (GetAtIndex<Col, RowList>::value == EMPTY);
};

template <typename Board>
struct CheckWin{
    typedef typename Board gameBoard;
    constexpr static int redCarColIdx = FindCar<X, gameBoard>::X_col_idx;
    constexpr static int redCarRowIdx = FindCar<X, gameBoard>::X_row_idx;
    typedef typename GetAtIndex<redCarRowIdx, gameBoard>::value redCarRow;
    constexpr static bool result = CheckWinHelper<redCarRow, redCarColIdx + 1, gameBoard::width - redCarColIdx - 1>::result;
};

#endif // RUSHHOUR_H