#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <unistd.h>


#define AGENTS_COUNT 200
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60
#define RECORD_SECONDS 10

struct Agent {
    int x, y;
    int direction;
    int direction_steps;
    SDL_Color color;
};

void reset_agents(std::vector<Agent>& agents) {
    agents.clear();
    for (int i = 0; i < AGENTS_COUNT; i++) {
        agents.push_back({
            WINDOW_WIDTH / 2 + (rand() % 21 - 10),
            WINDOW_HEIGHT / 2 + (rand() % 21 - 10),
            rand() % 4,

            0,
            { Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255 }
        });
    }
}

void RandomWalk() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Random Walk Visualization",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    std::vector<Agent> agents;
    reset_agents(agents);


    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                reset_agents(agents);
            }
        }

        for (auto& agent : agents) {
            int ox = agent.x, oy = agent.y;

            switch (agent.direction) {
                case 0: agent.y--; break;
                case 1: agent.x++; break;
                case 2: agent.y++; break;

                case 3: agent.x--; break;
            }


            if (agent.x < 0) agent.direction = 1;
            if (agent.x >= WINDOW_WIDTH) agent.direction = 3;
            if (agent.y < 0) agent.direction = 2;
            if (agent.y >= WINDOW_HEIGHT) agent.direction = 0;

            if (++agent.direction_steps >= 25) {
                agent.direction = rand() % 4;
                agent.direction_steps = 0;
            }

            SDL_SetRenderDrawColor(renderer, agent.color.r, agent.color.g, agent.color.b, 255);
            SDL_RenderDrawLine(renderer, ox, oy, agent.x, agent.y);
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void RandomWalkFfmpeg() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Random Walk Visualization (FFmpeg)",

        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);


    std::vector<Agent> agents;
    reset_agents(agents);

    std::vector<uint8_t> pixels(WINDOW_WIDTH * WINDOW_HEIGHT * 4);


    const int total_frames = FPS * RECORD_SECONDS;
    for (int frame = 0; frame < total_frames; frame++) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;
        }

        for (auto& agent : agents) {
            int ox = agent.x, oy = agent.y;

            switch (agent.direction) {
                case 0: agent.y--; break;

                case 1: agent.x++; break;
                case 2: agent.y++; break;
                case 3: agent.x--; break;
            }

            if (agent.x < 0) agent.direction = 1;
            if (agent.x >= WINDOW_WIDTH) agent.direction = 3;
            if (agent.y < 0) agent.direction = 2;
            if (agent.y >= WINDOW_HEIGHT) agent.direction = 0;

            if (++agent.direction_steps >= 25) {
                agent.direction = rand() % 4;

                agent.direction_steps = 0;

            }

            SDL_SetRenderDrawColor(renderer, agent.color.r, agent.color.g, agent.color.b, 255);
            SDL_RenderDrawLine(renderer, ox, oy, agent.x, agent.y);
        }

        SDL_RenderPresent(renderer);

        SDL_RenderReadPixels(
            renderer,
            nullptr,
            SDL_PIXELFORMAT_RGBA32,
            pixels.data(),
            WINDOW_WIDTH * 4

        );

        write(STDOUT_FILENO, pixels.data(), pixels.size());
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv) {

    if (argc > 1 && std::string(argv[1]) == "ffmpeg") {
        RandomWalkFfmpeg();
    } else {
        RandomWalk();
    }
    return 0;
}
