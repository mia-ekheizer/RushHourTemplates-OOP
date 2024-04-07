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


template<typename Board, typename moveList>
struct CheckSolution {
    //"returns" result: if moveList wins the game
};

//specialization for GameBoard and List<moves>
template<typename Board, typename... moves>
struct CheckSolution<GameBoard<Board>, List<moves...>> {
    typedef typename List<moves...> list;
    typedef typename GameBoard<Board> board;
    typedef typename list::head currMove;
    constexpr static int currCol = FindCar<currMove::type, board>::X_col_idx;
    constexpr static int currRow = FindCar<currMove::type, board>::X_row_idx;
    constexpr static Direction currDirection = currMove::direction;
    constexpr static int currSteps = currMove::amount;
    typedef typename moveVehicle<board, currRow, currCol, currDirection, currSteps>::board currBoard;
    //after first move in the list
    constexpr static bool result = CheckSolution<currBoard, list::next>::result;
};

template<typename Board>
struct CheckSolution<GameBoard<Board>, List<>> {
    constexpr static bool result = CheckWin<GameBoard<Board>>::result;
};

#endif // RUSHHOUR_H