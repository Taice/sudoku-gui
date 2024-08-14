# SUDOKU-GUI

## dependencies
### raylib:
#### ARCH BASED
install raylib package using aur helper
#### ANYTHING ELSE
i have no clue youre on your own

### make (ONLY INSTALL IF YOU PLAN ON BUILDING PROJECT YOURSELF):
#### ARCH BASED
install the base-devel package
#### DEBIAN BASED ( for the most part )
install the build-essential package

## how to run
`make run` inside the git directory 

## how to use
v - visual mode, press number after selecting an area - all selected nodes get that number added as a node <br>
i - insert mode - default mode, press number to insert it into the grid at the current node <br>
n - notes mode, press number to insert a note into this node <br>
m - missing mode - shows all the digits missing in your B: (box) R: (row) and C: (col) <br><br>
y - highlight mode - after pressing a number highlights all cells with that number and all notes of that number
rr - to reset board <br>
cc - if youre cheating you can do this to auto generate notes. (toggle-able)<br>

vim keybinds for movement


## TROUBLESHOOTING

If app randomly stops edit the framerate.h file to a higher refresh rate.
