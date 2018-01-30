from Const import Const
from Move import Move
from Game import Game
from GamePlay import GamePlay
from BasicAgent import BasicAgent
from DepthLimitedAgent import DepthLimitedAgent

class DepthVsWeightedGamePlay(GamePlay):
    def createGame(self): return Game()
    def createAgentO(self): return DepthLimitedAgent(Const.MARK_O,1)
    def createAgentX(self): return BasicAgent(Const.MARK_X)

if __name__=='__main__':
    gameplay= DepthVsWeightedGamePlay()
    gameplay.play()
    input()

