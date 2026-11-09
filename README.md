#### Progetto Fondamenti di Reti Neurali

Progetto per l'esame del corso di Fondamenti di Reti Neurali tenuto dal Prof. G. Buttazzo nell'A.A. 2025/2026.

Questo progetto usa raylib: https://github.com/raysan5/raylib/releases

font: https://www.dafont.com/retro-gaming.font

## Build

Requires gcc, make and the X11/OpenGL development libraries (raylib is vendored in `raylib/`).

```
make          # builds bin/game and bin/train
make run      # builds and launches the game
make clean
```

Controls: `Left`/`Right` rotate, `Space` thrust (and restart after a game end).
