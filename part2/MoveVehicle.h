#ifndef CODE_MOVEVEHICLE_H
#define CODE_MOVEVEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "BoardCell.h"
#include "TransposeList.h"
#include "GameBoard.h"
#include "Utilities.h"


using namespace std;

/***************************   MOVE   **************************/
template<CellType Type, Direction Dir, int Amount>
struct Move {
    static_assert(Type != EMPTY, "cannot move an EMPTY cell!");
    constexpr static CellType type = Type;
    constexpr static Direction direction = Dir;
    constexpr static int amount = Amount;
};



// Find_Car_Helper Class Declaration
// This class recursively iterates over the board's cells and finds the coordinates of a cell containing an end (the front or back) of the car "type" (note that the first end to be found depends on the direction of the search).
// type - the car to find
// cyrr_type - the car on the current cell
// (curr_row, curr_col) - the coordinates of the current cell
// done - search is done
// board_main_list - main list of the board
template <CellType type, CellType curr_type, int curr_row, int curr_col, bool done, typename board_main_list> 
struct Find_Car_Helper{
    typedef board_main_list mainList;
    static constexpr bool last_row = (mainList::size == curr_row + 1);
    static constexpr bool found = (curr_type == type);
    static constexpr bool last_cell_in_board = (last_row && (curr_col == 0));
    static_assert(!(!found && last_cell_in_board), "Type was not found!");
    static constexpr int next_row = ConditionalInteger<last_row, 0, curr_row + 1>::value; // this is the next cell's row
    static constexpr int next_col1 = ConditionalInteger<last_row, curr_col - 1, curr_col>::value; // this is the next cell's column
    static constexpr int next_col = ConditionalInteger<last_cell_in_board, 0, next_col1>::value;
    typedef typename GetAtIndex<next_row, mainList>::value next_row_list;
    typedef typename Conditional<last_cell_in_board, BoardCell<EMPTY, UP, 0>, typename GetAtIndex<next_col, next_row_list>::value>::value next_cell;
    typedef Find_Car_Helper<type, next_cell::type, next_row, next_col, found, mainList> next_helper;

    static constexpr int X_row = ConditionalInteger<found, curr_row, next_helper::X_row>::value;
    static constexpr int X_col = ConditionalInteger<found, curr_col, next_helper::X_col>::value;
};

// Find_Car_Helper Specialization - stopping condition
template <CellType type, CellType curr_type, int curr_row, int curr_col, typename board_main_list>
struct Find_Car_Helper<type, curr_type, curr_row, curr_col, true, board_main_list> {
    constexpr static int X_col = curr_col;
    constexpr static int X_row = curr_row;
};

// FindCar Class Declaration
// (uses Find_Car_Helper to find car "type" in board "Board" - read Find_Car_Helper for more info)
template<CellType type, typename Board>
struct FindCar{
    typedef Board game_board;
    typedef typename game_board::board mainList;
    typedef typename GetAtIndex<0, mainList>::value subList; //first row of the board
    static constexpr int last_col_idx = game_board::width - 1;
    typedef typename GetAtIndex<last_col_idx, subList>::value first_cell; //top right cell

    // TODO: do I need "next_helper" or just the find_car_helper?
    // was ::next_helper car_loc, changed to car_loc
    typedef Find_Car_Helper<type, first_cell::type, 0, last_col_idx, false, mainList> car_loc;
    static constexpr int X_row_idx = car_loc::X_row;
    static constexpr int X_col_idx = car_loc::X_col;
};

// Dir Class Declaration
// This class computes the further end of a car respect to "car_direction" given the end found using FindCar.
// (e.g. if "car_direction"=RIGHT, the further end of the car is the left end...)
// c - a direction
// (Row, Col) - a random end of the car
// len - the car's length
template<Direction car_direction, int Row, int Col, int len>
struct Dir{};

// Dir Specialization (you need to implement more specializations similarly)
template<int Row, int Col, int len>
struct Dir<RIGHT, Row, Col, len> {
    static constexpr int row_i = Row;
    static constexpr int col_i = Col - len + 1;
};

template<int Row, int Col, int len>
struct Dir<LEFT, Row, Col, len> {
    static constexpr int row_i = Row;
    static constexpr int col_i = Col;
};

template<int Row, int Col, int len>
struct Dir<UP, Row, Col, len> {
    static constexpr int row_i = Row + len - 1;
    static constexpr int col_i = Col;
};

template<int Row, int Col, int len>
struct Dir<DOWN, Row, Col, len> {
    static constexpr int row_i = Row;
    static constexpr int col_i = Col;
};

// direct Class Declaration
// This class recursively moves a car "count" steps on the board in a certain direction
// direction - the direction of the movement
// counter - recursive counter (remaining amount of steps)
// mainBoardList - main list of the board
// car_cell - a cell on the current board containing the car to be moved
// (Row, Col) - coordinates of the further end of the car respect to "d" (e.g. if "d"=RIGHT, the further end of the car is the left end...)
template<Direction direction, int counter, typename mainBoardList, typename car_cell, int Col, int Row>
struct direct{};

// direct Specialization (you need to implement more specializations similarly)
template<int counter, typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<RIGHT, counter, mainBoardList, car_cell, Col, Row>{
    typedef typename direct<RIGHT, counter - 1, mainBoardList, car_cell, Col, Row>::moved mainList; // main list of the board after we moved the car "count"-1 steps
    typedef typename GetAtIndex<Row, mainList>::value subList;
    // this is the closer end (respect to "d") after the #"count" step
    typedef typename GetAtIndex<Col + counter + car_cell::length - 1, subList>::value destCellFront;
    static_assert(destCellFront::type == EMPTY, "Error, Collision cell MoveVehicle");
    typedef BoardCell<EMPTY, UP, 0> emptyCell;
    typedef typename SetAtIndex<Col + counter - 1, emptyCell, subList>::list subListAfterRemovingBack; // sublist after changing left end
    typedef typename SetAtIndex<Col + counter + car_cell::length - 1, car_cell, subListAfterRemovingBack>::list subListAfterMovingFront; // sublist after changing right end
    typedef typename SetAtIndex<Row, subListAfterMovingFront, mainList>::list boardAfterMove;
    typedef boardAfterMove moved;
};

// direct Specialization - stop condition
template<typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<RIGHT, 0, mainBoardList, car_cell, Col, Row> {
    typedef mainBoardList moved;
};

template<int counter, typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<LEFT, counter, mainBoardList, car_cell, Col, Row>{
    typedef typename direct<LEFT, counter - 1, mainBoardList, car_cell, Col, Row>::moved mainList;
    typedef typename GetAtIndex<Row, mainList>::value subList;
    typedef typename GetAtIndex<Col - counter - car_cell::length + 1, subList>::value destCellFront;
    static_assert(destCellFront::type == EMPTY, "Error, Collision cell MoveVehicle");
    typedef BoardCell<EMPTY, UP, 0> emptyCell;
    typedef typename SetAtIndex<Col - counter + 1, emptyCell, subList>::list subListAfterRemovingBack;
    typedef typename SetAtIndex<Col - counter - car_cell::length + 1, car_cell, subListAfterRemovingBack>::list subListAfterMovingFront;
    typedef typename SetAtIndex<Row, subListAfterMovingFront, mainList>::list boardAfterMove;
    typedef boardAfterMove moved;
};

template<typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<LEFT, 0, mainBoardList, car_cell, Col, Row> {
    typedef mainBoardList moved;
};

template<int counter, typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<UP, counter, mainBoardList, car_cell, Col, Row>{
    typedef typename Transpose<mainBoardList>::matrix transposedBoard;
    typedef typename direct<LEFT, counter, transposedBoard, car_cell, Row, Col>::moved transposedBoardAfterMove;
    typedef typename Transpose<transposedBoardAfterMove>::matrix moved;
};

template<int counter, typename mainBoardList, typename car_cell, int Col, int Row>
struct direct<DOWN, counter, mainBoardList, car_cell, Col, Row>{
    typedef typename Transpose<mainBoardList>::matrix transposedBoard;
    typedef typename direct<RIGHT, counter, transposedBoard, car_cell, Row, Col>::moved transposedBoardAfterMove;
    typedef typename Transpose<transposedBoardAfterMove>::matrix moved;
};


// MoveVehicle Class Declaration
template<typename gameBoard, int R, int C, Direction D, int A>
struct MoveVehicle{};

// MoveVehicle Specialization
template<typename B, int R1, int C1, Direction Dl, int A>
struct MoveVehicle<GameBoard<B>, R1, C1, Dl, A>{

    typedef GameBoard<B> PrevBoard;
    typedef typename PrevBoard::board mainList;
    typedef GetAtIndex<R1, mainList> subList;
    typedef GetAtIndex<C1, typename subList::value> cell;
    typedef typename cell::value my_cell; //my_cell is of kind BoardCell

    static_assert(R1 >= 0 && R1 < PrevBoard::length, "Error Row,Move");
    static_assert(C1 >=0 && C1 < PrevBoard::width, "Error column,Move");
    static_assert(my_cell::type != EMPTY, "Error,empty cell MoveVehicle");
    constexpr static bool is_horizontal = ((Dl == RIGHT || Dl == LEFT) && (my_cell::direction == RIGHT || my_cell::direction == LEFT));
    constexpr static bool is_vertical = ((Dl == UP || Dl == DOWN) && (my_cell::direction == UP || my_cell::direction == DOWN));
    static_assert(is_horizontal || is_vertical, "Error,direction cell MoveVehicle");

    static constexpr int R2 = FindCar<my_cell::type, PrevBoard>::X_row_idx;
    static constexpr int C2 = FindCar<my_cell::type, PrevBoard>::X_col_idx;
    // the further end:
    static constexpr int R3 = Dir<Dl, R2, C2, my_cell::length>::row_i;
    static constexpr int C3 = Dir<Dl, R2, C2, my_cell::length>::col_i;

    typedef typename direct<Dl, A, B, my_cell, C3, R3>::moved o1;
    typedef GameBoard<o1> board;

};

#endif //CODE_MOVEVEHICLE_H
