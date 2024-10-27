#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

//add new lib function not std c lib
void itoa(unsigned int n, char *buf) {
    int i = 0;
    
    // 特殊处理0的情况
    if (n == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return;
    }

    // 提取每一位，存入buf（倒序存储）
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    
    buf[i] = '\0'; // 添加字符串结束符

    // 反转buf中的字符
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
}

void xtoa(unsigned int n, char *buf) {
    int i = 0;

    // 处理 n 为 0 的特殊情况
    if (n == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return;
    }

    // 提取每一位并倒序存入 buf
    while (n > 0) {
        int remainder = n % 16;
        if (remainder < 10) {
            buf[i++] = remainder + '0';
        } else {
            buf[i++] = remainder - 10 + 'a';
        }
        n /= 16;
    }

    buf[i] = '\0'; // 结束符

    // 反转字符串
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
}

void *malloc(size_t size) {
  // On native, malloc() will be called during initializaion of C runtime.
  // Therefore do not call panic() here, else it will yield a dead recursion:
  //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
  panic("Not implemented");
#endif
  return NULL;
}

void free(void *ptr) {
}

#endif
