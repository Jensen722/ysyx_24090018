#include <am.h>
#include <nemu.h>
#include <klib.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  /* int i;
 int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
   uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
   for (i = 0; i < w * h; i ++) fb[i] = i;
     outl(SYNC_ADDR, 1);*/
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint16_t width = inl(VGACTL_ADDR) >> 16;
  uint16_t height = inl(VGACTL_ADDR);

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, .height = height,
    .vmemsz = width * height * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
uint32_t *pixels = (uint32_t *)ctl->pixels;
  int w = ctl->w, h = ctl->h;
  int x_start = ctl->x, y_start = ctl->y;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      // 计算帧缓冲区中该像素的地址
      uintptr_t pixel_addr = FB_ADDR + ((y_start + y) * 400 + (x_start + x)) * sizeof(uint32_t);
      outl(pixel_addr, pixels[y * w + x]);
    }
  }
  /*uint32_t *pixels = (uint32_t *)ctl->pixels;
  int x = ctl->x / ctl->w;
 // int y = ctl->y / ctl->h;
  //printf("ADDR = %x\n", FB_ADDR + 4*x);
  //printf("y = %d\n", y);
  outl(FB_ADDR + 4*x, *(pixels+x));*/
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
