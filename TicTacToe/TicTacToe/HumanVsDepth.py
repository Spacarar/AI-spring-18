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
    userSide = Const.MARK_NONE
    strSide=""
    level = 0
    print("Lets play TicTacToe")
    while(userSide==Const.MARK_NONE):
        strSide = input("pick a side (x or o): ")
        if(strSide=="x"or strSide=="X"):
            userSide=Const.MARK_X
        elif(strSide=="o" or strSide=="O"):
            userSide=Const.MARK_O
        else: 
            strSide=""
            print("invalid side. (x or o)")
    while(level<=0):
        level = int(input("Recursion level (high values will be slower): "))
    if(userSide==Const.MARK_O):
        gameplay.setAgentX(gameplay.myAgentX(level))
    else:
        gameplay.setAgentO(gameplay.myAgentO(level))
    gameplay.playVS(userSide)
    
       
        

