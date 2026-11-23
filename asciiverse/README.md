# Ambassador of Pain

**Navigate the Ball into the at-sign!**

That is all level 1 asks of you, and it sounds easy. Your ball, the red
`O`, never stops rolling. An arrow key sends it off in a new direction
and it keeps going that way until you send it somewhere else, or until
it hits something. Everything on the screen that is not empty space is a
wall: the borders, the boxes, the giant letters, every star and every
word of the level's own text. Touch any of it and you lose a life and
start over, five lives in all.

Every crash leaves something behind. A `0` appears where your ball last
rolled, and a `0` is an extra life for whoever rolls over it, so a hard
corner slowly fills up with spare lives. Each level starts you off with a
pile of points that drains away every moment you play, and every press of
an arrow costs 77 more: the quickest line with the fewest turns scores
best.

Eleven levels, each a plain page of text. Roll past the game's own name
spelled out in letters of X, through a maze of walls folded inside each
other, across a field of stars with a narrow staircase cut through it,
into a grid of walls made of the very `O`s your ball is made of, and up
the four layers of a network stack to the last level, where the at-sign
is the one in THE AMBASSAD@R 0F PAIN.

## Sixty-four lines

The whole game is 64 lines of C. Claire Wolf once told Raffael
Himmelreich that C64 programmers used to write games like her own
*Power Ballon* in under a hundred lines, and that on a modern Unix it
would take twice that. Raffael wrote this one in 97 lines to prove
otherwise. Claire sent it back almost at once, squeezed to 64. They
published it in 2004, versions 0.1 to 0.6 between January and April,
with an open invitation: new levels and nifty features are welcome, as
long as the code stays at 64 lines or fewer. The last line of the file
still says it: "Yup - This are 64 lines of C code. ;-)"

## Playing

Run `aop` in a terminal of at least 80x25.

- **Arrow keys**: send the ball up, down, left or right
- **q**: quit

The game ends with "Well done!" after level 11, "Sucker!" when the last
life is gone, or "Bye." when you quit, and the score. Upstream's own
README, with the story in Raffael's words, is `TRIVIA.md`.

## Your own levels

A level is a text file of 24 lines of 80 characters, like
`levels/empty-level.txt`: `%` is where the ball starts, `@` the goal,
`0` an extra life, and anything else is a wall. Play your own with
`aop mylevel.txt` (or several, in order: `aop one.txt two.txt`).

## Credits

- Ambassador of Pain: Raffael Himmelreich and Claire Wolf
  (https://github.com/hit-sys/Ambassador-Of-Pain), GNU GPL version 3
  (`LICENSE`)
- Packaged for Asciiverse: the launcher that finds the levels beside the
  program and the header that centres the field are ours (`PATCHES.md`);
  the 64 lines are theirs, unchanged

## Building

`sh asciiverse/build.sh` leaves the program in `build/<platform>/`;
static Linux builds run it in Alpine (`docker run --rm -v "$PWD":/src -w
/src alpine:3.22 sh asciiverse/build.sh`), Windows in an MSYS2 shell
with PDCurses. `asciiverse/package.sh` makes the av archive and
`.github/workflows/asciiverse-release.yml` builds every release.
