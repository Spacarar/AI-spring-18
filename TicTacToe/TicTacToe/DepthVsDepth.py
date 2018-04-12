from Const import Const
from Move import Move
from Game import Game
from GamePlay import GamePlay
from BasicAgent import BasicAgent
from DepthLimitedAgent import DepthLimitedAgent

class DepthVsDepthGamePlay(GamePlay):
    def createGame(self): return Game()
    def createAgentO(self): return DepthLimitedAgent(Const.MARK_O,1)
    def createAgentX(self): return DepthLimitedAgent(Const.MARK_X,4)

    def myAgentO(self,level):
        if level<=0:
            raise ValueError("level cannot be 0 or less")
        if level>5:
            print("high values may cause major slowdown")
        return DepthLimitedAgent(Const.MARK_O,level)
    def myAgentX(self,level):
        if level<=0:
            raise ValueError("level cannot be 0 or less")
        if level>5:
            print("high values may cause major slowdown")
        return DepthLimitedAgent(Const.MARK_X,level)

if __name__=='__main__':
    gameplay= DepthVsDepthGamePlay()
    levelX = 0
    levelO = 0
    print("set the recursion level of the players, higher values will be slower")
    while(levelX<=0 and levelO<=0):
        levelX=int(input("X recursion level: "))
        levelO=int(input("O recursion Level: "))
    gameplay.setAgentO(gameplay.myAgentO(levelO))
    gameplay.setAgentX(gameplay.myAgentX(levelX))
    gameplay.play()
    

