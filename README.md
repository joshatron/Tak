Tak
===

Plays the game of tak, from kingkiller chronicle.
Instruction are included in rules.pdf.

To run: "scons -Q"
Then run executable

TODO:

* make gui for game
* test engine
    * placing
        * illegal piece
        * out of pieces
        * each piece
            * empty square
            * on each piece
            * off board
    * moving
        * start
            * empty spot
            * off board
            * not player's spot
            * player's spot
        * move
            * leave all on first square
            * leave intermediate with no squares
            * cover standing without capstone
            * cover standing with capstone and more
            * cover capstone
            * move off board
            * leave 1 on each
            * leave multiple on each
            * leave differing on each
    * end of game
        * road
            * serpentine road
            * straight road
            * 2 not opposite sides
            * with capstones
            * with standing stones
            * enemy spot in middle
        * board coverage
            * white has more
            * black has more
            * same number
            * stacks
            * varying pieces
        * board full
            * board full
            * board not full
        * no pieces
            * white empty
            * white no capstones
            * white no normal pieces
            * black empty
            * black no capstones
            * black no normal pieces
* import/export
    * starting board setup
    * export match
