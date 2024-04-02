template<CellType cell_type, Direction direction, int len_vehicle>
struct BoardCell {
    CellType type = cell_type;
    Direction direction = direction;
    int length = len_vehicle;
};