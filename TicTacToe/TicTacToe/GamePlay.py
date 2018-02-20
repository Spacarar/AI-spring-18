from Const import Const
from Move import Move
from Game import Game
from BasicAgent import BasicAgent

class GamePlay:
    def __init__(self):
        self._game=None
        self._agentO=None
        self._agentX=None
    def getGame(self):
        if self._game==None:
            self._game=self.createGame()
        return self._game
    def getAgentO(self):
        if self._agentO==None:
            self._agentO=self.createAgentO()
        return self._agentO
    def getAgentX(self):
        if self._agentX==None:
            self._agentX=self.createAgentX()
        return self._agentX

    def setAgentX(self,agX):self._agentX=agX
    def setAgentO(self,agO):self._agentO=agO

    def turn(self):
        game= self.getGame()
        if game.oTurn():
            move = self.getAgentO().move(game)
            print(move)
            move.play(game)
        elif game.xTurn():
            move = self.getAgentX().move(game)
            print(move)
            move.play(game)
        else:
            raise ValueError("Invalid game state ("+Const.stateStr(game.getState())+")")
    def play(self):
        game = self.getGame()
        while not game.over():
            self.turn()
            game.printBoard()
        print(Const.stateStr(game.getState()))
        game.printBoard()
    def playVS(self,userSide):
        game=self.getGame()
        strMove=""
        while not game.over():
            game.printBoard()
            if (game.xTurn() and userSide==Const.MARK_X) or \
            (game.oTurn() and userSide==Const.MARK_O):
                while(not self.goodString(strMove)):
                    strMove=input("user: ")
                Move.play(Move.parse(strMove),game)
                strMove=""
            else:
                print("AI TURN")
                self.turn()
            print("")
            print(Const.stateStr(game.getState()))
        game.printBoard()


    def goodString(self, str):
        if str=="" or str=="\n" or str ==" ":
            return False
        return True