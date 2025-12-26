#include <bits/stdc++.h>
#include <SDL2/SDL.h>

#define AGENTS_COUNT 200
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Agent {
    int x, y;
    int direction;
    int direction_steps;
    SDL_Color color;
};

void reset_agents(void* data) {
    auto* agents = static_cast<std::vector<Agent>*>(data);
    agents->clear();
    for (int i = 0; i < AGENTS_COUNT; i++) {
        agents->push_back({
            WINDOW_WIDTH/2 + (rand()%21 - 10),
            WINDOW_HEIGHT/2 + (rand()%21 - 10),
            rand() % 4,
            0,
            { Uint8(rand()%256), Uint8(rand()%256), Uint8(rand()%256), 255 }
        });
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Random Walk Visualization",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_SetWindowResizable(window, SDL_FALSE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    std::vector<Agent> agents;
    for(int i=0;i<AGENTS_COUNT;i++){
        agents.push_back({
            WINDOW_WIDTH/2+(rand() % 21 - 10),
            WINDOW_HEIGHT/2+(rand() % 21 - 10),
            rand() % 4,
            0,
            { (Uint8)(rand() % 256), (Uint8)(rand() % 256), (Uint8)(rand() % 256), 255 }
        });
    }

    bool running = true;

    while(running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                reset_agents(&agents);
            }
        }

        for(auto&agent:agents) {
            int oldX = agent.x;
            int oldY = agent.y;

            switch(agent.direction) {
                case 0: agent.y -= 1; break; // Up
                case 1: agent.x += 1; break; // Right
                case 2: agent.y += 1; break; // Down
                case 3: agent.x -= 1; break; // Left
            }

            if(agent.y >= WINDOW_HEIGHT) agent.direction = 0;
            if(agent.x < 0) agent.direction = 1;
            if(agent.y < 0) agent.direction = 2;
            if(agent.x >= WINDOW_WIDTH) agent.direction = 3;

            agent.direction_steps++;

            if(agent.direction_steps >= 25) {
                agent.direction = rand() % 4;
                agent.direction_steps = 0;
            }

            SDL_SetRenderDrawColor(renderer, agent.color.r, agent.color.g, agent.color.b, agent.color.a);
            SDL_RenderDrawLine(renderer, oldX-1, oldY-1, agent.x+1, agent.y+1);
            // SDL_RenderDrawLine(renderer, oldX+1, oldY-1, agent.x-1, agent.y+1);
            SDL_RenderDrawLine(renderer, oldX, oldY, agent.x, agent.y);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


