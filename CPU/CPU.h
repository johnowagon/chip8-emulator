#include <SDL2/SDL.h>

class CPU {
  unsigned short opcode;
  unsigned char memory[4096];//
  unsigned char v[16];//registers 0-15. v[15] is used for carry flag
  unsigned short I;//index register
  unsigned short pc;
  unsigned short display[64*32];//starts with all zeros.
  SDL_Window *_window;//window we will update.
  SDL_Renderer* renderer;
  unsigned char sound_timer;
  unsigned char delay_timer;
  unsigned short stack[16];//used to keep track of where we are when we jump.
  unsigned short sp;//stack pointer
  bool drawFlag;
  void createRect();
  void updateDebug();
public:
  unsigned char key[16];
  bool step; //controls stepping through functionality.
  bool loadROM(const char* filename);//must be done before initialization!
  void listMemory();
  void emulateCycle();
  void initialize();
  void destroy();
};
