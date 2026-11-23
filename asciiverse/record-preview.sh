#!/bin/sh
# Records the catalog preview, asciiverse/ambassador-of-pain.cast
# (hub/RECORDING.md): levels 1 to 4 played by asciiverse/drive.py, which
# steers the ball along routes from asciiverse/plan.py, then quitting on
# level 5. The 80x25 field sits in the middle of the 100x30 recording: the
# game runs in an 80x25 tmux session shown in a borderless popup of the
# 100x30 session asciinema records (own sockets, no user config). Needs
# asciinema 3, tmux 3.2 or newer and the built binary.
#   asciiverse/record-preview.sh build/linux/aop
set -eu
binary=$(cd "$(dirname "$1")" && pwd)/$(basename "$1")
here=$(cd "$(dirname "$0")" && pwd)
repo=$(dirname "$here")
out=$here/ambassador-of-pain.cast
home=$(mktemp -d)
trap 'tmux -L avin kill-server 2>/dev/null; tmux -L avrec kill-server 2>/dev/null; rm -rf "$home"' EXIT
mkdir -p "$home/bin/levels"
cp "$binary" "$home/bin/aop"
cp "$repo"/aop-level-*.txt "$home/bin/levels/"
cd "$home"  # nothing in the recording names a local path
export HOME="$home" XDG_STATE_HOME="$home/state" PATH="$home/bin:$PATH" \
  USER=player LOGNAME=player TERM=xterm-256color COLORTERM=truecolor LANG=C.UTF-8
tmux -L avin -f /dev/null new-session -d -s in -x 80 -y 25 "aop" \; set -g status off
tmux -L avrec -f /dev/null new-session -d -s rec -x 100 -y 30 \
  "while tmux -L avin has-session 2>/dev/null; do sleep 0.2; done" \; set -g status off
python3 -I "$here/drive.py" "$home/bin/levels" &
drive=$!
asciinema rec --headless --overwrite -q \
  --window-size 100x30 --idle-time-limit 2 \
  --capture-env "TERM,COLORTERM,LANG" --title "Ambassador of Pain" \
  --command "tmux -L avrec attach -t rec" "$out"
wait "$drive"
