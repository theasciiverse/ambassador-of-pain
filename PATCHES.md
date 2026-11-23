# Changes from upstream

This is the Asciiverse packaging of
[Ambassador of Pain](https://github.com/hit-sys/Ambassador-Of-Pain) by
Raffael Himmelreich and Claire Wolf, a curses arcade game in 64 lines of
C. Upstream is the `upstream` remote, Raffael's own repository (2020);
this build is its `master` at `c3f00fd` (2020-08-03), which holds release
0.6. The `asciiverse` branch adds files around the game and changes none
of upstream's.

## Licence

Raffael's repository carries the GNU GPL version 3 (`LICENSE`); the 0.6
tarball's `COPYING` (GPL version 2) is kept beside it, and `aop.c` says
"Free Software under the terms of GNU GPL". As an author's own choice,
version 3 stands: the catalog lists `GPL-3.0-only` and the archives ship
`LICENSE`.

## The game

Unchanged, every byte: `aop.c` is still 64 lines, and it and all eleven
levels, `empty-level.txt` and `COPYING` are identical to the original
`aop-0.6.tar.gz` of 2004-04-14 (sha256
68a8b6b8f08216b31895d536f7dc4beb137a787fd17096da2d46fd863497f65a, from
raffi.at/code/aop/ through the Internet Archive) and to the 2017 import at
github.com/buribu/aop. Only upstream's Makefile differs from the tarball's
(it now builds through `aop.o`), and we do not use it.

## Around it

- **Levels beside the program** (`asciiverse/launch.c`, new): started
  with no arguments, upstream looks for its levels in
  `/usr/local/share/aop`. `aop.c` is compiled with `-Dmain=aop_main` and
  linked with this small launcher, which supplies the real `main`: it
  finds the eleven levels in `levels/` beside the program (the av archive)
  or in `../share/ambassador-of-pain/levels` (the system packages) and
  hands them to the game in order. Arguments are passed on untouched, so
  `aop mylevel.txt` works as before.
- **The score stays readable** (same file): the game prints its last word
  and the score after leaving the screen. Started with no arguments from
  a terminal, the launcher then waits for Enter, so the score is not wiped
  at once by av taking the screen back.
- No saves, settings or logs: the game writes nothing, so there is no
  state folder.

## Build settings

`asciiverse/build.sh` compiles `aop.c` and the launcher directly. Linux
links ncurses statically in Alpine, so the binary needs nothing installed
and reads the system's terminfo; macOS links the system ncurses; Windows
links PDCurses statically from MSYS2 (MinGW's `unistd.h` has the
`usleep` and `sleep` the game calls).

## Packaging

- `asciiverse/build.sh`, `asciiverse/launch.c` (new): the build above.
- `asciiverse/package.sh` (new): the av archive, with the program, the
  levels in `levels/`, our `README.md`, upstream's README as `TRIVIA.md`,
  `LICENSE` and this file.
- `asciiverse/nfpm.yaml` (new): .deb, .rpm, .apk and Arch packages named
  `ambassador-of-pain`, with the program as `/usr/bin/aop` (upstream's
  name) and the levels in `/usr/share/ambassador-of-pain/levels`.
- `.github/workflows/asciiverse-release.yml` (new): av archives for Linux
  (static, x86-64 and ARM64), macOS (Intel and Apple silicon) and Windows
  (x86-64 and ARM64), the system packages and SHA256SUMS, on a `*-av.*`
  tag.
- `asciiverse/README.md` (new): the package README.
- `asciiverse/record-preview.sh`, `asciiverse/drive.py`,
  `asciiverse/plan.py` (new): the catalog preview, levels 1 to 4 played
  along routes the planner finds.
- `asciiverse/ambassador-of-pain.toml`, `asciiverse/ambassador-of-pain.md`,
  `asciiverse/ambassador-of-pain.cast` (new): the catalog manifest, store
  page and preview, as drafts for hub.
- `PATCHES.md` (new): this file.
