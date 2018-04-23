# AI-spring-18

repository for AI class, spring 2018

#### TicTacToe source code direct link
    -https://github.com/Spacarar/AI-spring-18/tree/TicTacToe/TicTacToe/TicTacToe

#### Conways Game source code direct link
    -https://github.com/Spacarar/AI-spring-18/tree/TicTacToe/ConwaysGame/ConwaysGame
 
###### Conways Game Exit Note:

currently when exiting the program, the GridPlayer search thread can hang until evaluation of the current pattern is complete, this will usually resolve in a few seconds.
   
###### CONWAYS GAME GRIDSIZE BUG NOTE:

currently an extremely large bug exists in which small grids somehow mess up the map container to either add extranous or lose data. size of map seems to reset to 257 elements at random (lose data if size was greater, adds data if size was smaller). I am unsure the origins of the bug and have spent hours trying to add safety features, or ensure proper addition of new grids. However, with grids 20X20 or larger this does not seem to be an issue for some reason. The logic behind this error is unclear to me as it's origin is unknown. As values are added it appears correct then suddenly size() will return 257 and continue to mess up values theron afterwards.
