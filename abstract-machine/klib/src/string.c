#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //panic("Not implemented");
  size_t len = 0;
  for(; *s != '\0';s++){
    len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  //panic("Not implemented");
  char *str = dst;
  for(; *src != '\0'; src++){
    *str = *src;
    str++;
  }
  *str = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  //panic("Not implemented");
  for(size_t i = 0; i < n; i++){
    *(dst+i) = *(src+i);
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  //panic("Not implemented");
  char *dst_end = dst + strlen(dst);
  strcpy(dst_end, src);
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  //panic("Not implemented");
  while(*s1 && (*s1 == *s2)){
    s1++;
    s2++;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  //panic("Not implemented");
  while(n > 0 && *s1 && (*s1 == *s2)) {
    s1++;
    s2++;
    n--;
  }
  if(n == 0){
    return 0;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void *memset(void *s, int c, size_t n) {
  //panic("Not implemented");
  unsigned char *byte_ptr = (unsigned char *)s;
  unsigned char val = (unsigned char)c;
  for(size_t i = 0; i < n; i++){
    byte_ptr[i] = val;
  }
  return s;

}

void *memmove(void *dst, const void *src, size_t n) {
  //panic("Not implemented");
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;

    // 如果源地址和目标地址重叠，并且目标地址在源地址之后
    // 从后往前复制，以避免覆盖源数据
    if (d > s && d < s + n) {
        d += n;
        s += n;
        while (n--) {
            *(--d) = *(--s);
        }
    } 
    // 否则直接从前往后复制
    else {
        while (n--) {
            *d++ = *s++;
        }
    }
    return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  //panic("Not implemented");
	unsigned char* dst = (unsigned char*)out;
  const unsigned char* s = (const unsigned char *)in; 
	while (n--)
	{
		*(dst++) = *(s++);
	}
	return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  //panic("Not implemented");
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;
  // 逐字节比较内存区域
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
        // 返回两字节的差值
      return p1[i] - p2[i];
        }
    }

    // 如果所有字节都相等，返回 0
    return 0;
}

#endif
