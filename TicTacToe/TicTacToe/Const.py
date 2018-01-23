class Const:
    ROWS=3
    COLS = 3
    MARK_NONE = 0
    MARK_O =1
    MARK_X =2
    STATE_TURN_O=1
    STATE_TURN_X = 2
    STATE_WIN_O=3
    STATE_WIN_X=4
    STATE_DRAW=5
    def rowOK(row):
        if not isinstance(row,int):
            raise ValueError("row must be an integer")
        if row<0 or row >=Const.ROWS:
            
