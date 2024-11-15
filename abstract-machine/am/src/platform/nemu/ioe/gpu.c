#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
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
  uint32_t *pixels = (uint32_t *)ctl->pixels;
  int w = ctl->w, h = ctl->h;
  int x_start = ctl->x, y_start = ctl->y;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      // 计算帧缓冲区中该像素的地址
      uintptr_t pixel_addr = FB_ADDR + ((y_start + y) * io_read(AM_GPU_CONFIG).width + (x_start + x)) * sizeof(uint32_t);
      outl(pixel_addr, pixels[y * w + x]); //输出绘制区域的像素
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
