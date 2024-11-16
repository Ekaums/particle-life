#include <iostream>
#include <SDL.h>
#include <SDL_rect.h>

const int SCREEN_W{640};
const int SCREEN_H{480};

bool init(SDL_Window * &window, SDL_Renderer * &render){

    // Init
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Could not initialize. Error: " << SDL_GetError() << '\n'; // GetError tells you any errors from SDL
        return false;
    }

    // Create window
    window = SDL_CreateWindow("Particle Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    if(window == nullptr){
        std::cout << " Window could not be created. Error: " << SDL_GetError() << '\n';
        return false;
    }

    // Get window surface to manipulate
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!render) {
        std::cout << "Renderer could not be created SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }

    return true;
}

int main(){

    // Window to be rendered
    SDL_Window *window{};
    SDL_Renderer *render;
    if(init(window, render) == false){
      exit(1);
    }

    
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black
    SDL_RenderClear(render); // ?

   
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255); // Red

    SDL_Rect particleRect = {SCREEN_W/2, SCREEN_H/2, 10, 10};
    
    SDL_RenderFillRect(render, &particleRect);
    SDL_RenderPresent(render);

    //Hack to get window to stay up
    SDL_Event e; bool quit = false; while(quit == false){while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT) quit = true;} }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

