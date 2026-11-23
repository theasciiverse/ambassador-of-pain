"""Finds a way through an Ambassador of Pain level, for the preview driver
(asciiverse/drive.py): the ball keeps rolling the way it was last sent,
so a route is a list of turns, each a cell and the arrow to press when the
ball is on it. Moving horizontally takes 50 ms a cell and vertically
100 ms, as in aop.c; every turn costs a little extra so routes keep to
few, unhurried turns (at least three cells apart).
    python3 -I plan.py aop-level-01.txt ...   (prints each level's route)
"""
import heapq
import sys

DIRS = {"Up": (0, -1), "Down": (0, 1), "Left": (-1, 0), "Right": (1, 0)}
TURN = 0.6
MINRUN = 3


def load(path):
    """the field as aop.c reads it: 81 bytes a row, '%' the start"""
    data = open(path, "rb").read().decode("latin-1")
    field = [[" "] * 81 for _ in range(25)]
    start = None
    x = y = 0
    for ch in data:
        if y > 24:
            break
        if ch == "%":
            start = (x, y)
        else:
            field[y][x] = ch
        x = (x + 1) % 81
        if x == 0:
            y += 1
    return field, start


def route(field, start):
    """[(cell, key), ...]: the first entry is the start and the first key"""
    def free(x, y):
        return 0 <= y < 25 and 0 <= x < 80 and field[y][x] in " 0@"
    queue = [(0.0, start, None, MINRUN, ())]
    seen = set()
    while queue:
        cost, (x, y), d, run, turns = heapq.heappop(queue)
        if field[y][x] == "@":
            return list(turns)
        if ((x, y), d, run) in seen:
            continue
        seen.add(((x, y), d, run))
        for k, (dx, dy) in DIRS.items():
            turning = k != d
            if turning and run < MINRUN:
                continue
            nx, ny = x + dx, y + dy
            if not free(nx, ny):
                continue
            step = 0.05 if dx else 0.1
            heapq.heappush(queue, (
                cost + step + (TURN if turning else 0), (nx, ny), k,
                1 if turning else min(run + 1, MINRUN),
                turns + (((x, y), k),) if turning else turns))
    return None


if __name__ == "__main__":
    for p in sys.argv[1:]:
        f, s = load(p)
        r = route(f, s)
        print(p, s, None if r is None else (len(r), r))
