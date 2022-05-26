#include "./CPU/CPU.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char**argv){
  CPU chip8;
  SDL_Event e;
  bool quit = false;
  chip8.initialize();
  if(!chip8.loadROM(argv[1])){
    return -1;
  }
  while (!quit){
      while (SDL_PollEvent(&e) != 0){
          if (e.type == SDL_QUIT){
              quit = true;
          }
          else if (e.type == SDL_KEYDOWN &&e.key.repeat == 0 ){
              switch(e.key.keysym.sym){
                  case SDLK_1:
                      chip8.key[0x1] = 1;
                  break;

                  case SDLK_2:
                      chip8.key[0x2] = 1;
                  break;

                  case SDLK_3:
                      chip8.key[0x3] = 1;
                  break;

                  case SDLK_4:
                      chip8.key[0xC] = 1;
                  break;

                  case SDLK_q:
                      chip8.key[0x4] = 1;
                  break;

                  case SDLK_w:
                      chip8.key[0x5] = 1;
                  break;

                  case SDLK_e:
                      chip8.key[0x6] = 1;
                  break;

                  case SDLK_r:
                      chip8.key[0xD] = 1;
                  break;

                  case SDLK_a:
                      chip8.key[0x7] = 1;
                  break;

                  case SDLK_s:
                      chip8.key[0x8] = 1;
                  break;

                  case SDLK_d:
                      chip8.key[0x9] = 1;
                  break;

                  case SDLK_f:
                      chip8.key[0xE] = 1;
                  break;

                  case SDLK_z:
                      chip8.key[0xA] = 1;
                  break;

                  case SDLK_x:
                      chip8.key[0x0] = 1;
                  break;

                  case SDLK_c:
                      chip8.key[0xB] = 1;
                  break;

                  case SDLK_v:
                      chip8.key[0xF] = 1;
                  break;

                  case SDLK_SPACE:
                    chip8.step = !chip8.step;
                  break;

                  case SDLK_RIGHT:
                    if(chip8.step){
                      chip8.emulateCycle();
                      std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }else{
                      ;;
                    }
                  break;

                  case SDLK_ESCAPE:
                      exit(0);
                  break;
              }
          }
          else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
              switch(e.key.keysym.sym){
                  case SDLK_1:
                      chip8.key[0x1] = 0;
                  break;

                  case SDLK_2:
                      chip8.key[0x2] = 0;
                  break;

                  case SDLK_3:
                      chip8.key[0x3] = 0;
                  break;

                  case SDLK_4:
                      chip8.key[0xC] = 0;
                  break;

                  case SDLK_q:
                      chip8.key[0x4] = 0;
                  break;

                  case SDLK_w:
                      chip8.key[0x5] = 0;
                  break;

                  case SDLK_e:
                      chip8.key[0x6] = 0;
                  break;

                  case SDLK_r:
                      chip8.key[0xD] = 0;
                  break;

                  case SDLK_a:
                      chip8.key[0x7] = 0;
                  break;

                  case SDLK_s:
                      chip8.key[0x8] = 0;
                  break;

                  case SDLK_d:
                      chip8.key[0x9] = 0;
                  break;

                  case SDLK_f:
                      chip8.key[0xE] = 0;
                  break;

                  case SDLK_z:
                      chip8.key[0xA] = 0;
                  break;

                  case SDLK_x:
                      chip8.key[0x0] = 0;
                  break;

                  case SDLK_c:
                      chip8.key[0xB] = 0;
                  break;

                  case SDLK_v:
                      chip8.key[0xF] = 0;
                  break;
            }
      }
  }
    if(!chip8.step){
      chip8.emulateCycle();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
  chip8.destroy();
  return 0;
}
