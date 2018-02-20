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
            if self.side==Const.MARK_O: ans =(level+1)+Const.WIN_VALUE
            else: ans=(-level)-Const.WIN_VALUE
        elif game.xWon():
            if self.side == Const.MARK_X: ans=(level+1)+Const.WIN_VALUE
            else: ans =(-level)-Const.WIN_VALUE
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
                    ans=max(ans,moveValue)
                else:
                    ans=min(ans,moveValue)
        if ans==0:ans = self.weighBoard(game)
        return (ans,myOptions)

    def move(self,game):
        (maxValue,maxOptions)=(0,0)
        if game.isEmptyBoard():
            return Move(random.randint(0,Const.ROWS-1),random.randint(0,Const.COLS-1),self.side)
        
        if self.level>0:
            (maxValue,maxOptions)=self.value(game,self.level)
            
        else:
            print("critical error: no maxValue could be calculated")
       
        if(maxValue,maxOptions)==(0,0):
            raise ValueError("level should not be 0 here!")
        
        playable = []
        maxPlayableOption =0
        valueList = []
        for move in game.getMoves():
            move.play(game)
            (moveValue,moveOptions)=self.value(game, self.level)
            print("testing: "+str(move)," value = "+str(moveValue)+"  options = "+str(moveOptions))
            move.unplay(game)
            valueList.append(moveValue)
            #had to subtract 1, if too high sometimes a better move than current state does not exist
            if moveValue>=maxValue:
                playable.append((move,moveOptions))
            maxPlayableOption = max(maxPlayableOption,moveOptions)
        print(valueList)
        bestPlayable=[]
       # if moveValue<=0:
        #    print("unfavorable conditions ahead")
         #   print("Looking for better option nearby")
          #  return self.nearbySpot(game,self.side)
      

        for (move, options) in playable:
            if options >= maxPlayableOption:
                bestPlayable.append(move)
        
        if len(bestPlayable)>0:
            spot=random.randint(0,len(bestPlayable)-1)
            print("i found a good spot")
            for m in bestPlayable:
             print("moves:  "+str(m))
            return bestPlayable[spot]
        
        elif len(playable)>0:
            spot = random.randint(0,len(playable)-1)
            print("i found an alright spot")
            for m in playable:
                print("possible: " +str(m))
            return playable[spot][0]
        
        else:
            print("No moves are better than right now,LOW WIN CHANCE")
            return game.getMoves()[random.randint(0,len(game.getMoves())-1)]
    
    
    def nearbySpot(self, game,side):
        mySpots=[]
        myPlayable=[]
        for row in range(Const.ROWS):
            for col in range(Const.COLS):
                if game._board[row][col]==side:
                    mySpots.append((row,col))
                if game._board[row][col]==Const.MARK_NONE:
                    myPlayable.append((row,col))
        bestSpot=None
        bestScore=0
        for play in myPlayable:
            playscore=0
            for spot in mySpots:
                if play[0]==spot[0]-1 or play[0]==spot[0]+1 and \
                play[1]==spot[1]-1 or play[0]==spot[1]+1:
                    playscore+=1
            if playscore>bestScore:
                bestSpot=play
                bestScore=playscore
        return Move(bestSpot[0],bestSpot[1],side)

    def weighBoard(self,game):
        ans=0
        enemyPositions=[]
        selfPositions=[]
        if game.isEmptyBoard(): return 0
        if(self.side==Const.MARK_O):
            enemyPositions=game.getXPositions()
            selfPositions=game.getOPositions()
        elif(self.side==Const.MARK_X):
            enemyPositions=game.getOPositions()
            selfPositions=game.getXPositions()
        else:raise valueError("Agent must have side")
        
        for ePos in enemyPositions:
            if(game.countRow(ePos[0],ePos[1])>=Const.WIN_VALUE-1):
               ans-=game.countRow(ePos[0],ePos[1])
            elif(game.countCol(ePos[0],ePos[1])>=Const.WIN_VALUE-1):
                ans-=game.countCol(ePos[0],ePos[1])
            elif(game.countDiag(ePos[0],ePos[1])>=Const.WIN_VALUE-1):
                ans-=game.countDiag(ePos[0],ePos[1])
            elif(game.countOffDiag(ePos[0],ePos[1])>=Const.WIN_VALUE-1):
                ans-=game.countOffDiag(ePos[0],ePos[1])
        for sPos in selfPositions:
            if(game.countRow(sPos[0],sPos[1])>=Const.WIN_VALUE-1):
               ans+=game.countRow(sPos[0],sPos[1])
            elif(game.countCol(sPos[0],sPos[1])>=Const.WIN_VALUE-1):
                ans+=game.countCol(sPos[0],sPos[1])
            elif(game.countDiag(sPos[0],sPos[1])>=Const.WIN_VALUE-1):
                ans+=game.countDiag(sPos[0],sPos[1])
            elif(game.countOffDiag(sPos[0],sPos[1])>=Const.WIN_VALUE-1):
                ans+=game.countOffDiag(sPos[0],sPos[1])
        return ans