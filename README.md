# Random Walk Visualization

A simple SDL2 + C++ simulation of multiple agents performing a **random walk**, leaving colorful trails as they move across the screen. Each agent periodically changes direction, creating emergent, organic patterns over time.

<p align="center">
  <img src="RandomWalk.gif" alt="Random Walk Demo" />
</p>

## Compile

g++ RandomWalk.cpp -std=c++17 -O2 `sdl2-config --cflags --libs` -o RandomWalk

## Run

# Normal interactive mode
./RandomWalk

# FFmpeg capture mode (GIF)
./RandomWalk ffmpeg | ffmpeg -f rawvideo -pixel_format rgba -video_size 800x600 -framerate 60 -i - RandomWalk.gif
