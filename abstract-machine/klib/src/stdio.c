#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
int printf(const char *fmt, ...) {
    char buf[64];
    int buf_len = 0;
    int len = 0;
    va_list ap;
    va_start(ap, fmt);


    len = vsprintf(buf, fmt, ap);
    for(int i = 0; buf[i] != '\0'; i++){
      putch(buf[i]);
      buf_len++;
    }
    va_end(ap);
    return len;
    /*
    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            fmt++;
            switch(*fmt)
         {
                case 'd': //整型
                {
                        int num = va_arg(ap, int);
                        assert(num != 0);
                        if(num < 0)
                        { 
                           putch('-');
                           len++;
                           assert(num == (int)-2147483648);
                           if(num == -2147483648){ //打印最小整数有问题
                             const char *min_int_str = "2147483648";
                             for(int i = 0; min_int_str[i] != '\0'; i++){
                               putch(min_int_str[i]);
                               len++;
                             }
                            } else{
                            num = -num;
                            } //可能会出问题，当num=-2147483648
                        }
                        if(num == 0){
                          putch('0');
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          putch(buf[i]);
                          len++;
                        }
                        break;
                }    
                case 'c': //字符型
                {
                        char c = va_arg(ap, int);
                        putch(c);
                        len++;
                        break;
                }
                case 'x': //16进制
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          putch('0');
                          len++;
                        }
                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'a';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          putch(buf[i]);
                          len++;
                        }
                        break;
                }
                case 'X': //16进制0-9 A-F
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          putch('0');
                          len++;
                        }
                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'A';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          putch(buf[i]);
                          len++;
                        }
                        break;
                }
                case 'u': //输出无符号整数
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          putch('0');
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          putch(buf[i]);
                          len++;
                        }
                        break;
                }
                case 's': //字符串
                {
                        char *s = va_arg(ap, char *);
                        for(int i = 0; s[i] != '\0'; i++){
                         putch(s[i]);
                         len++;
                        }
                        break;
                }
                case '%': //输出%//
                {
                    putch('%');
                    len++;
                    break;
                }
                default:
                        break;
            }
          }
        else
        {  
            putch(*fmt);
            len++;
            if(*fmt == '\n')
            {
                    
            }
        }
        fmt++;
    }
    va_end(ap);
    return len;*/
}


int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
/*功能：向字符串 格式化打印一个字符串
*参数：格式化的字符串
*注意：这个是简易版本 (%02x 完成)
* %-3s不行， %f也不行， %X不行
*/
    int len = 0;
    char buf[64];
    int buf_len = 0;
    

    
    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            fmt++;
            switch(*fmt)
         {
                case 'd': //整型
                {
                        int num = va_arg(ap, int);
                        if(num < 0)
                        {   
                            if(out != NULL){
                              out[len] = '-';
                            }
                            len++;
                            if(num == -2147483648){
                              memcpy(out+len, "2147483648", 10);
                              len += 10;
                            } else{
                            num = -num;} //可能会出问题，当num=-2147483648
                        }
                        if(num == 0){
                          if(out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          if(out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }    
                case 'c': //字符型
                {
                        char c = va_arg(ap, int);
                        if(out != NULL){
                          out[len] = c;
                        }
                        len++;
                        break;
                }
                case 'x': //16进制
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'a';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          if(out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }
                case 'X': //16进制0-9 A-F
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'A';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          if(out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }
                case 'u': //输出无符号整数
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          if(out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }    
                case 's': //字符串
                {
                        char *s = va_arg(ap, char *);
                        for(int i = 0; s[i] != '\0'; i++){
                          if(out != NULL){
                            out[len] = s[i];
                          }
                          len++;
                        }
                        break;
                }
                case '%': //输出%//
                {
                    if(out != NULL){
                      out[len] = '%';
                    }
                    len++;
                    break;
                }
                default:
                        break;
            }
        }
        else
        {  
            if(out != NULL){
              out[len] = *fmt;
            }
            len++;
            if(*fmt == '\n')
            {
                    
            }
        }
        fmt++;
    }
    if(out != NULL){
      out[len] = '\0';
    }

    return len;

}

/*
*功能：整型(int) 转化成 字符型(char)
*注意：不用 % / 符号的话，只能正确打印:0...9的数字对应的字符'0'...'9'
*/

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  return cnt;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    int len = 0;
    char buf[64];
    int buf_len = 0;
    

    
    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            fmt++;
            switch(*fmt)
         {
                case 'd': //整型
                {
                        int num = va_arg(ap, int);
                        if(num < 0)
                        {   
                            if(len < n - 1 && out != NULL){
                              out[len] = '-';
                            }
                            len++;
                            num = -num;
                        }
                        if(num == 0){
                          if(len < n - 1 && out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          if(len < n - 1 && out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }    
                case 'c': //字符型
                {
                        char c = va_arg(ap, int);
                        if(len < n -1 && out != NULL){
                          out[len] = c;
                        }
                        len++;
                        break;
                }
                case 'x': //16进制
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(len < n - 1 && out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'a';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          if(len < n - 1 && out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }
                case 'X': //16进制0-9 A-F
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(len < n - 1 && out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){ //倒序记录
                          int remainder = num % 16;
                          if(remainder < 10){
                            buf[buf_len++] = remainder + '0';
                          } else{
                            buf[buf_len++] = remainder - 10 + 'A';
                          }
                          num /= 16;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中
                          if(len < n - 1 && out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }
                case 'u': //输出无符号整数
                {
                        uint32_t num = (uint32_t)va_arg(ap, int);
                        if(num == 0){
                          if(len < n - 1 && out != NULL){
                            out[len] = '0';
                          }
                          len++;
                        }

                        buf_len = 0;
                        while(num > 0){  //倒序记录 如123记录到buf中的结果是321
                          buf[buf_len++] = (num % 10) + '0';
                          num /= 10;
                        }
                        for(int i = buf_len - 1; i >= 0; i--){ //倒序输出到out中，321->123
                          if(len < n - 1 && out != NULL){
                            out[len] = buf[i];
                          }
                          len++;
                        }
                        break;
                }    
                case 's': //字符串
                {
                        char *s = va_arg(ap, char *);
                        for(int i = 0; s[i] != '\0'; i++){
                          if(len < n - 1 && out != NULL){
                            out[len] = s[i];
                          }
                          len++;
                        }
                        break;
                }
                case '%': //输出%//
                {
                    if(len < n - 1 && out != NULL){
                      out[len] = '%';
                    }
                    len++;
                    break;
                }
                default:
                        break;
            }
        }
        else
        {  
            if(len < n - 1 && out != NULL){
              out[len] = *fmt;
            }
            len++;
            if(*fmt == '\n')
            {
                    
            }
        }
        fmt++;
    }
    if(n != 0 && out != NULL){
      if(len < n){
        out[len] = '\0';
      } else{
        out[n-1] = '\0';
      }
    }

    return len;

}

#endif
