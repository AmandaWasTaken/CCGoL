A CLI implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)

It's not a great one but I was just bored so I made it as a pastime mini project

Currently only supports a perfect square board size

Build requires any C compiler (even better if you have Make)

# TODO
- Figure out why loop detection is broken on large boards (tested with 25x25, didn't work)
- Allow the user to choose starting position instead of filling the board with randomized values
- Optimize the loop detection algorithm because it sucks ass right now
- Fix bugs and change/remove stupid decisions in the code (there are a lot of them) 
