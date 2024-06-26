#ifndef RUSHHOUR_H
#define RUSHHOUR_H

#include "BoardCell.h"
#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "List.h"
#include "MoveVehicle.h"
#include "Utilities.h"

template <typename RowList, int Col, int Counter>
struct CheckWinHelper {
    constexpr static bool tmp = CheckWinHelper<RowList, Col + 1, Counter - 1>::result;
    constexpr static bool result = ConditionalInteger<GetAtIndex<Col, RowList>::value::type == EMPTY, tmp, false>::value;
};

template <typename RowList, int Col>
struct CheckWinHelper<RowList, Col, 0> {
    constexpr static bool result = true;
};

template <typename Board>
struct CheckWin{
    typedef Board gameBoard;
    constexpr static int redCarColIdx = FindCar<X, gameBoard>::X_col_idx;
    constexpr static int redCarRowIdx = FindCar<X, gameBoard>::X_row_idx;
    typedef typename GetAtIndex<redCarRowIdx, typename gameBoard::board>::value redCarRow;
    constexpr static bool result = CheckWinHelper<redCarRow, redCarColIdx + 1, gameBoard::width - redCarColIdx - 1>::result;
};

template<typename Board, typename moveList>
struct CheckSolution {};

template<typename Board>
struct CheckSolution<Board, List<>> {
    constexpr static bool result = CheckWin<Board>::result;
    //static_assert(1==2, "should be here");
};

//specialization for GameBoard and List<moves>
template<typename Board, typename currMove, typename... moves>
struct CheckSolution<Board, List<currMove, moves...>> {
    //static_assert(1==2, "should NOT be here");
    typedef List<moves...> list;
    typedef Board mainBoard;
    constexpr static int currCol = FindCar<currMove::type, mainBoard>::X_col_idx;
    constexpr static int currRow = FindCar<currMove::type, mainBoard>::X_row_idx;
    constexpr static Direction currDirection = currMove::direction;
    constexpr static int currSteps = currMove::amount;
    typedef typename MoveVehicle<Board, currRow, currCol, currDirection, currSteps>::board currBoard;
    //after first move in the list
    constexpr static bool result = CheckSolution<currBoard, list>::result;
};

#endif // RUSHHOUR_H