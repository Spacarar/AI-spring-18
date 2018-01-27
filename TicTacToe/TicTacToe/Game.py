from Const import Const
from Move import Move

class Game:
    def over(self):
        return \
            self._state == Const.STATE_WIN_O or \
            self._state == Const.STATE_WIN_X or \
            self._state == Const.STATE_DRAW

    def moveOk(self, row, col, mark):
        Const.rowOk(row)
        Const.colOk(col)
        Const.markOk(mark)
        if (self.xTurn()) and (mark ==Const.MARK_X) and \
            self._board[row][col]==Const.MARK_NONE: 
            return

        if(self.oTurn()) and (mark == Const.MARK_O) and \
           self._board[row][col]==Const.MARK_NONE: 
            return
        raise ValueError("move invalid in current state!")

#added these functions to simplify some more code on higher levels
    def xTurn(self):
        return self._state == Const.STATE_TURN_X
    def oTurn(self):
        return self._state==Const.STATE_TURN_O
    def xWon(self):
        return self._state==Const.STATE_WIN_X
    def oWon(self):
        return self._state==Const.STATE_WIN_O
    def draw(self):
        return self._state==Const.STATE_DRAW

    def _repeats(self,row,col,rowDir,colDir):
        mark = self._board[row][col]        
        count = 0
        while (row >= 0 and row < Const.ROWS and col >= 0) and \
              col < Const.COLS and self._board[row][col]==mark:
            row=row+rowDir
            col=col+colDir
            count = count + 1
        return count

    def _length(self,row,col,rowDir,colDir):
        return self._repeats(row,col,rowDir,colDir)+self._repeats(row,col,-rowDir,-colDir)-1

    def _winRow(self,row,col):
        return self._length(row,col,1,0) >= 3
    def _winCol(self,row,col):
        return self._length(row,col,0,1) >= 3
    def _winMainDiag(self,row,col):
        return self._length(row,col,1,1) >= 3
    def _winOffDiag(self,row,col):
        return self._length(row,col,1,-1) >= 3

    def _win(self,row,col):
        return self._winRow(row,col) or self._winCol(row,col) or \
               self._winMainDiag(row,col) or self._winOffDiag(row,col)

    def _draw(self):
        return self._unplayed == 0

    def getMoves(self):
        mark = None
        if self._state == Const.STATE_TURN_O:
            mark = Const.MARK_O
        elif self._state == Const.STATE_TURN_X:
            mark = Const.MARK_X

        if mark == None:
            return []

        moves = []
        for row in range(Const.ROWS):
            for col in range(Const.COLS):
                if self._board[row][col] == Const.MARK_NONE:
                    moves.append(Move(row,col,mark))

        return moves

    def move(self,row,col,mark):
        self.moveOk(row,col,mark)
        self._board[row][col]=mark
        self._unplayed = self._unplayed - 1
        if self._win(row,col):
            if mark == Const.MARK_O:
                self._state = Const.STATE_WIN_O
            if mark == Const.MARK_X:
                self._state = Const.STATE_WIN_X
        elif self._draw():
            self._state = Const.STATE_DRAW
        else:
            if mark == Const.MARK_O:
                self._state = Const.STATE_TURN_X
            if mark == Const.MARK_X:
                self._state = Const.STATE_TURN_O

    def unmove(self,row,col):
        Const.rowOk(row)
        Const.colOk(col)
        if self._board[row][col] == Const.MARK_X:
            self._unplayed = self._unplayed + 1
            self._board[row][col] = Const.MARK_NONE
            self._state = Const.STATE_TURN_X
        if self._board[row][col] == Const.MARK_O:
            self._unplayed = self._unplayed + 1
            self._board[row][col] = Const.MARK_NONE
            self._state = Const.STATE_TURN_O
            
    def reset(self):
        self._board = [[Const.MARK_NONE for col in range(Const.COLS)] for row in range(Const.ROWS)]

        self._state = Const.STATE_TURN_O
        self._unplayed = Const.ROWS*Const.COLS

    def getBoard(self):
        return [[self._board[row][col]  for col in range(Const.COLS)] for row in range(Const.ROWS)]

    def isEmptyBoard(self):
        for col in range(Const.COLS):
            for row in range(Const.ROWS):
                if self._board[row][col]==Const.MARK_O or self._board[row][col]==Const.MARK_X:
                    return False
        return True

        
    def getState(self):
        return self._state
    
    def __init__(self):
        self.reset()

    def play(self,moves):
        for move in moves.split():
            Move.parse(move).play(self)