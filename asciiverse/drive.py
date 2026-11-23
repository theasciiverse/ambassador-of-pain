"""Plays Ambassador of Pain for the catalog preview (record-preview.sh).

The game runs in its own 80x25 tmux session (socket avin), shown in a
borderless popup in the middle of the 100x30 session asciinema records
(socket avrec). This reads the game's pane and steers the ball along the
routes plan.py finds through levels 1 to 4, pressing each arrow as the
ball reaches its turn, then quits on level 5 and leaves the score on
screen for a moment.
    python3 -I drive.py <levels folder>
"""
import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import plan  # noqa: E402

LEVELS = sys.argv[1]
PLAY = 4                     # levels to play
fields = []
for i in range(1, 12):
    fields.append(plan.load(os.path.join(LEVELS, "aop-level-%02d.txt" % i)))


def tmux(sock, *args):
    return subprocess.run(["tmux", "-L", sock, *args], capture_output=True,
                          text=True).stdout


def screen():
    rows = tmux("avin", "capture-pane", "-p", "-t", "in").split("\n")
    return [(r + " " * 80)[:80] for r in rows[:25]] + [" " * 80] * (25 - len(rows[:25]))


def which_level(rows):
    best, score = None, -1
    for i, (field, _) in enumerate(fields):
        s = sum(rows[y][x] == field[y][x] for y in range(1, 24) for x in range(80))
        if s > score:
            best, score = i, s
    return best


def ball(rows, field):
    for y in range(1, 24):
        for x in range(80):
            if rows[y][x] == "O" and field[y][x] != "O":
                return x, y
    return None


def key(k):
    tmux("avin", "send-keys", "-t", "in", k)


# wait for asciinema's client, then open the game in the middle of it
for _ in range(100):
    if tmux("avrec", "list-clients").strip():
        break
    time.sleep(0.1)
subprocess.Popen(["tmux", "-L", "avrec", "display-popup", "-B", "-x", "10",
                  "-y", "28", "-w", "80", "-h", "25", "-E",
                  "tmux -L avin attach -t in"])
time.sleep(1.5)

routes = [plan.route(*fields[i]) for i in range(PLAY)]
level, pressed, last = None, set(), None
while True:
    rows = screen()
    now = which_level(rows)
    if now >= PLAY:
        break
    field, start = fields[now]
    pos = ball(rows, field)
    if now != level:
        level, pressed, last = now, set(), None
        time.sleep(1.2)          # a look at the new level first
        continue
    if pos == start and last not in (None, start):
        pressed = set()          # lost a life: back at the start, standing still
    turns = dict(routes[now])
    if pos in turns and pos not in pressed:
        key(turns[pos])
        pressed.add(pos)
    last = pos
    time.sleep(0.008)
time.sleep(2.0)                  # level 5's title
key("q")
time.sleep(2.5)                  # "Bye." and the score
key("Enter")
time.sleep(1.0)
