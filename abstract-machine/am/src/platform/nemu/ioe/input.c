#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t key = inl(KBD_ADDR);  
  if(key == AM_KEY_NONE){
    kbd->keydown = false;
    kbd->keycode = AM_KEY_NONE;
  } else{
    if(key & KEYDOWN_MASK){
      kbd->keydown = true;
      kbd->keycode = key & (0x7FFF);
    } else{
      kbd->keydown = false;
      kbd->keycode = key;
      }
    }
}
