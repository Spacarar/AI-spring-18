import random
from Const import Const
from Move import Move
from Game import Game

class DepthLimitedAgent:
    def __init__(self, side,level):
        if side!=Const.MARK_O and side!=Const.MARK_X:
            raise ValueError("side must be X or O")
        if level<0:
            raise ValueError("Level must be 0 or greater")
        self.side=side
        self.level=level
    
    def value(self,game, level):
        ans = None
        (moveValue,moveOptions)=(0,0)
        if game.oWon():
            if self.side==Const.MARK_O: ans =1
            else: ans=-1
        elif game.xWon():
            if self.side == Const.MARK_X: ans = 1
            else: ans =-1
        elif game.draw():
            ans=0
        #if the answer was written, the game is in an ending state and ans is known
        if ans!=None: return(ans,0)
        
        iside =0
        if self.side==Const.MARK_O: iside=1
        else: iside=-1
        
        iturn =0
        if(game.oTurn()):iturn =1
        else: iturn =-1

        myTurn = (iside==iturn)
        myOptions =0
        for move in game.getMoves():
            if(level>0):
                move.play(game)
                (moveValue,moveOptions)=self.value(game,level-1)
                move.unplay(game)
                myOptions = myOptions + 1 + moveOptions
            if ans== None:
                ans=moveValue
            else:
                if myTurn:
                    ans = max(ans,moveValue)
                else:
                    ans = min(ans,moveValue)
        return (ans,myOptions)
    def move(self,game):
        (maxValue,maxOptions)=(0,0)
        if game.isEmptyBoard():
            return Move(random.randint(0,Const.ROWS-1),random.randint(0,Const.COLS-1),self.side)
        if self.level>0:
            (maxValue,maxOptions)=self.value(game,self.level)
        if(maxValue,maxOptions)==(0,0):
            raise ValueError("level should not be 0 here!")
        playable = []
        maxPlayableOption =0
        for move in game.getMoves():
            move.play(game)
            (moveValue,moveOptions)=self.value(game, self.level)
            move.unplay(game)
            #if moveValue==maxValue:
            playable.append((move,moveOptions))
            maxPlayableOption = max(maxPlayableOption,moveOptions)
        bestPlayable=[]
        for (move, options) in playable:
            if options == maxPlayableOption:
                bestPlayable.append(move)
        if len(bestPlayable)>0:
            spot=random.randint(0,len(bestPlayable)-1)
            return bestPlayable[spot]
        elif len(playable)>0:
            return playable[0]
        else:
            raise ValueError("no options found or error recursing")
    
    
