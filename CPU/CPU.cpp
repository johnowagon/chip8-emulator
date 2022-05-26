#include "CPU.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <ios>
#include <stdlib.h>

unsigned char chip8_fontset[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void CPU::emulateCycle(){
  opcode = memory[pc] << 8 | memory[pc + 1];
  printf("Opcode: 0x%x, PC: %d, SP: %d, Stack: %d, Delay: %d, Sound: %d\n", opcode, pc, sp, stack[sp], delay_timer, sound_timer);
  switch (opcode & 0xf000){
    case 0x0000:
    {
      switch (opcode & 0x000f){
        case 0x0000:
        {
          for(short i = 0; i< 64*32; ++i)
            display[i] = 0;
          drawFlag = true;
          pc += 2;
          break;
        }
        case 0x000e:
        {
          --sp;
          pc = stack[sp];
          pc += 2;
          break;
        }
        default:
          printf("Opcode not found: 0x%x\n", opcode);
      }
      break;
    }
    case 0x1000:
    {
      pc = opcode & 0x0fff;
      break;
    }
    case 0x2000:
    {
      stack[sp] = pc;
      ++sp;
      pc = opcode & 0x0fff;
      break;
    }
    case 0x3000:
    {
      if(v[(opcode & 0x0f00) >> 8] == (opcode & 0x00ff)){
        pc += 4;//skip next instruction
      }else{
        pc += 2;
      }
      break;
    }
    case 0x4000:
    {
      if(v[(opcode & 0x0f00) >> 8] != (opcode & 0x00ff)){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }
    case 0x5000:
    {
      if(v[(opcode & 0x0f00) >> 8] == v[(opcode & 0x00f0) >> 4]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }
    case 0x6000:
    {
      v[(opcode & 0x0f00) >> 8] = opcode & 0x00ff;
      pc += 2;
      break;
    }
    case 0x7000:
    {
      v[(opcode & 0x0f00) >> 8] += opcode & 0x00ff;
      pc += 2;
      break;
    }
    case 0x8000:
    {
      short x = (opcode & 0x0f00) >> 8;
      short y = (opcode & 0x00f0) >> 4;
      switch (opcode & 0x000f){
        case 0x0:
        {
          v[x] = v[y];//vx=vy
          pc += 2;
          break;
        }
        case 0x1:
        {
          v[x] = v[x] | v[y];
          pc += 2;
          break;
        }
        case 0x2:
        {
          v[x] = v[x] & v[y];
          pc += 2;
          break;
        }
        case 0x3:
        {
          v[x] = v[x] ^ v[y];
          pc += 2;
          break;
        }
        case 0x4:
        {
          v[x] = v[x] + v[y];
          if(v[x] > 0xff){
            v[0xf] = 1;
          }else{
            v[0xf] = 0;//unnecesary, but might as well.
          }
          pc += 2;
          break;
        }
        case 0x5:
        {
          if(v[x] > v[y]){
            v[0xf] = 1;
          }else{
            v[0xf] = 0;
          }
          v[x] = v[x] - v[y];
          pc += 2;
          break;
        }
        case 0x6:
        {
          v[0xf] = v[(opcode & 0x0f00) >> 8] & 0x1;
          v[(opcode & 0x0f00) >> 8] >>= 1;
          pc += 2;
          break;
        }
        case 0x7:
        {
          if(v[(opcode & 0x00f0) >> 4] > v[(opcode & 0x0f00) >> 8]){
            v[0xf] = 1;
          }else{
            v[0xf] = 0;
          }
          v[(opcode & 0x0f00) >> 8] = v[(opcode & 0x00f0) >> 4] - v[(opcode & 0x0f00) >> 8];
          pc += 2;
          break;
        }
        case 0xe:
        {
          v[0xf] = v[(opcode & 0x0f00) >> 8] >> 7;
          v[(opcode & 0x0f00) >> 8] <<= 1;
          pc += 2;
          break;
        }
        default:
          printf("Opcode not found: 0x%x\n", opcode);
      }
      break;
    }
    case 0x9000:
    {
      if(v[(opcode & 0x0f00) >> 8] != v[(opcode & 0x00f0) >> 4]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }
    case 0xA000:
    {
      I = opcode & 0x0fff;
      pc += 2;
      break;
    }
    case 0xB000:
    {
      pc = (opcode & 0x0fff) + v[0];
      break;
    }
    case 0xC000:
    {
      v[(opcode & 0x0f00) >> 8] = (rand() % 0xff) & (opcode & 0x00ff);
      pc += 2;
      break;
    }
    case 0xD000:
    {
      unsigned short x = v[(opcode & 0x0F00) >> 8];
      unsigned short y = v[(opcode & 0x00F0) >> 4];
      unsigned short height = opcode & 0x000F;
      unsigned short pixel;

      v[0xF] = 0;
      for (int yline = 0; yline < height; yline++)
      {
        pixel = memory[I + yline];
        for(int xline = 0; xline < 8; xline++)
        {
          if((pixel & (0x80 >> xline)) != 0)
          {
            if(display[(x + xline + ((y + yline) * 64))] == 1)
              v[0xF] = 1;
            display[x + xline + ((y + yline) * 64)] ^= 1;
          }
        }
      }

      drawFlag = true;
      pc += 2;
      break;
    }
    case 0xe000:
    {
      switch ( opcode & 0x00ff){
        case 0x9e:
        {
          if(key[v[(opcode & 0x0f00) >> 8]] == 1){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }
        case 0xa1:
        {
          if(key[v[(opcode & 0x0f00) >> 8]] != 1){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }
        default:
          printf("Opcode not found: 0x%x\n", opcode);
      }
      break;
    }
    case 0xf000:
    {
      switch (opcode & 0x00ff){
        case 0x07:
        {
          v[(opcode & 0x0f00) >> 8] = delay_timer;
          pc += 2;
          break;
        }
        case 0x15:
        {
          delay_timer = v[(opcode & 0x0f00) >> 8];
          pc += 2;
          break;
        }
        case 0x18:
        {
          sound_timer = v[(opcode & 0x0f00) >> 8];
          pc += 2;
          break;
        }
        case 0x1e:
        {
          if(I + v[(opcode & 0x0f00) >> 8] > 0xfff){
            v[0xf] = 1;
          }else{
            v[0xf] = 0;
          }
          I += v[(opcode & 0x0f00) >> 8];
          pc += 2;
          break;
        }
        case 0x0a:
        {
          bool keypress = false;
          for(int i = 0; i< 16; ++i){
              if (key[i] == 1) {
                  v[(opcode & 0x0F00) >> 8] = i;
                  keypress = true;
              }
          }
          if(!keypress)
              return;
          pc += 2;
          break;
        }
        case 0x29:
        {
          I = v[(opcode & 0x0F00) >> 8] *5;
          pc += 2;
          break; //wrong implementation? if bug look here!
        }
        case 0x33:
        {
          memory[I]  = v[(opcode & 0x0F00) >> 8] / 100;
          memory[I+1] = (v[(opcode & 0x0F00) >> 8] / 10) % 10;
          memory[I+2] = (v[(opcode & 0xF00) >> 8] % 100) % 10;
          pc += 2;
          break;
        }
        case 0x55:
        {
          unsigned char X = (opcode & 0x0f00) >> 8;
          for (unsigned char r = 0; r <= X; r++) {
              memory[I+r] = v[r];
          }

          I = I + X + 1;
          pc += 2;
          break;
        }
        case 0x65:
        {
          unsigned char X = ((opcode & 0x0F00) >> 8);
          for (unsigned char r = 0; r <= X; r++) {
              v[r] = memory [I+r];
          }
          I = I + X + 1;
          pc += 2;
          break;
        }
        default:
        {
          printf("Opcode not found: 0x%x\n", opcode);
          break;
        }
      }
      break;
    }
    default:
    {
      printf("Opcode not found: 0x%x\n", opcode);
      break;
    }
  }
  if(drawFlag){
    createRect();
  }
  if(delay_timer > 0)
    --delay_timer;

  if(sound_timer > 0){
    if (sound_timer == 1)
      --sound_timer;
  }
}

void CPU::initialize(){
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  this->_window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_RESIZABLE);
  this->renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
  std::fill((int*)this->display, (int*)this->display + sizeof(this->display)/sizeof(int), 0);

  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;
  step = false;
  //clear display
  for(int i = 0; i< 64*32; ++i)
    display[i] = 0;

  //clear stack
  for(int i = 0; i < 16; ++i)
    stack[i] = 0;

  //clear registers V0-VF
  for(int i = 0; i < 16; ++i)
    v[i] = 0;

  //clear memory
  for(int i = 0; i< 4096; ++i)
    memory[i] = 0;

  for(int i = 0; i < 80; ++i)
    memory[i] = chip8_fontset[i];

  drawFlag = false;
}

void CPU::createRect(){
  SDL_SetRenderDrawColor(renderer, 0x00, 0x0, 0x0, 0x0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff,0xff,0xff,0xFF);
  int rownum = 0;
  SDL_Rect pixel;
  for(int y = 0; y < 32; ++y){
      for(int x = 0; x < 64; ++x) {

          pixel.x = x*10;
          pixel.y = y*10;
          pixel.w = 10;
          pixel.h = 10;
          rownum = y*64;
          if(this->display[x + rownum] == 1){
              SDL_RenderFillRect(renderer,&pixel);
          }
      }
  }
  SDL_RenderPresent(renderer);
  drawFlag = false;
}

void CPU::updateDebug(){

}

bool CPU::loadROM(const char* filename){
  using namespace std;
  ifstream rom(filename, ios::binary | ios::ate);
  streamsize size = rom.tellg();
  rom.seekg(0, ios::beg);

  vector<char> buffer(size);

  if(rom.read(buffer.data(),size)){
    for (long i = 0; i < size; ++i)
		{
      memory[0x200 + i] = buffer[i];
		}
    rom.close();
    buffer.clear();
    printf("Rom loaded successfully.\n");
    return true;
  }else{
    rom.close();
    buffer.clear();
    printf("Rom didn't load.\n");
    return false;
  }
}

void CPU::listMemory(){
  for(int i = 0x200; i < 600; i+=2){
    printf("%x %x \n", memory[i], memory[i+1]);
  }
}

void CPU::destroy(){
  SDL_DestroyWindow(this->_window);
  SDL_Quit();
}
