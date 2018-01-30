class Const:
    ROWS=4
    COLS = 4
    MARK_NONE = 0
    MARK_O =1
    MARK_X =2
    STATE_TURN_O=1
    STATE_TURN_X = 2
    STATE_WIN_O=3
    STATE_WIN_X=4
    STATE_DRAW=5
    PRINT_X_O ={MARK_NONE:' ', MARK_O: 'O', MARK_X:'X'}


    def rowOk(row):
        if not isinstance(row,int):
            raise ValueError("row must be an integer")
        if row<0 or row >=Const.ROWS:
            raise ValueError("row ("+str(row)+") must be between 0 and " +str(Const.ROWS-1))
   
    def colOk(col):
        if not isinstance(col, int):
            raise ValueError("col must be an integer")
        if col<0 or col >=Const.COLS:
            raise ValueError("col ("+str(col)+") must be between 0 and " +str(Const.COLs-1))

    def markOk(mark):
        if not isinstance(mark, int):
            raise ValueError("Mark must be an integer")
        if mark != Const.MARK_NONE and mark != Const.MARK_O and mark != Const.MARK_X:
            raise ValueError("mark (" +str(mark)+") must be NONE O or X")
    def stateOK(state):
        if not isinstance(state, int):
            raise ValueError("state must be an integer")
        if (state != Const.STATE_TURN_X) and (state != Const.STATE_TURN_O) and \
           (state != Const.STATE_WIN_O) and (state != Const.STATE_WIN_X) and \
           (state != Const.STATE_DRAW):
                raise ValueError("state ("+str(state)+") must be either a turn, win, or draw")
    def stateStr(state):
        if state == Const.STATE_TURN_O: return "o's turn"
        if state == Const.STATE_TURN_X: return "x's turn"
        if state == Const.STATE_WIN_O: return "o won"
        if state == Const.STATE_WIN_X: return "x won"
        if state == Const.STATE_DRAW: return "draw"
        return "unknown state (" + str(state) + ")"

    def markStr(mark):
        if mark == Const.MARK_O: return "o"
        if mark == Const.MARK_X: return "x"
        if mark == Const.MARK_NONE: return " "
        return "unkown mark (" + str(mark) + ")"