from Const import Const
from Move import Move
from Game import Game
from GamePlay import GamePlay
from BasicAgent import BasicAgent

class HumanVsWeightedGamePlay(GamePlay):
    def createGame(self):return Game()
    def createAgentO(self): return BasicAgent(Const.MARK_O)
    def createAgentX(self): return BasicAgent(Const.MARK_X)

if __name__=='__main__':
    gameplay= HumanVsWeightedGamePlay()
    gameplay.playVS(Const.MARK_X)
