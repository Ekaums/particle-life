#include <iostream>
#include <SDL.h>

const int SCREEN_W{640};
const int SCREEN_H{480};

bool init(SDL_Window * &window, SDL_Surface * &surface){

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
  surface = SDL_GetWindowSurface(window);

  return true;
}

int main(){

  // Window to be rendered
  SDL_Window *window{};
  // Used to access surface in the window
  SDL_Surface *surface{};

  init(window, surface);

  // Fill it white
  SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );

  // Update the surface
  SDL_UpdateWindowSurface(window);

  //Hack to get window to stay up
  SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

  //Destroy window
  SDL_DestroyWindow( window );

  //Quit SDL subsystems
  SDL_Quit();

  return 0;
}

