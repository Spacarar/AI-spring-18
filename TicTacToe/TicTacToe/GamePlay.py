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
        print(Const.stateStr(game.getState()))
        game.printBoard()
    def playVS(self,userSide):
        game=self.getGame()
        while not game.over():
            if game.xTurn() and userSide==Const.MARK_X:
                print("user: ")
                Move.play(Move.parse(input()),game)
            elif game.oTurn() and userSide==Const.MARK_O:
                print("user: ")
                Move.play(Move.parse(input()),game)
            else:
                self.turn()
            print(Const.stateStr(game.getState()))
            game.printBoard()
