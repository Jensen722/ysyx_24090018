#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
int printf(const char *fmt, ...) {
  panic("Not implemented");
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
    char c;
    char *s;
    int n;
    
    //int index = 0;
    //int ret = 2;
    
    char buf[65];
    char digit[16];
    char *str = out;
    //int num = 0;
    //int len = 0;
    
    memset(buf, 0, sizeof(buf));
    memset(digit, 0, sizeof(digit));

    
    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            fmt++;
            switch(*fmt)
         {
                case 'd': //整型
                {
                        n = va_arg(ap, int);
                        if(n < 0)
                        {
                            *str = '-';
                            str++;
                            n = -n;
                        }
                        itoa(n, buf);
                        memcpy(str, buf, strlen(buf));
                        str += strlen(buf);
                        break;
                }    
                case 'c': //字符型
                {
                        c = va_arg(ap, int);
                        *str = c;
                        str++;
                        
                        break;
                }
                case 'x': //16进制
                {
                        n = va_arg(ap, int);
                        xtoa(n, buf);
                        memcpy(str, buf, strlen(buf));
                        str += strlen(buf);
                        break;
                }
                case 's': //字符串
                {
                        s = va_arg(ap, char *);
                        memcpy(str, s, strlen(s));
                        str += strlen(s);
                        break;
                }
                case '%': //输出%//
                {
                    *str = '%';
                    str++;
                    
                    break;
                }
                /*case '0': //位不足的左补0
                {
                        index = 0;
                        num = 0;
                        memset(digit, 0, sizeof(digit));
                        
                        while(1)
                        {
                                fmt++;
                                ret = isDigit(*fmt);
                                if(ret == 1) //是数字
                                {
                                        digit[index] = *fmt;
                                        index++;
                                }
                                else
                                {
                                        num = atoi(digit);
                                        break;
                                }
                        }
                        switch(*fmt)
                     {
                                case 'd': //整型
                                {
                                        n = va_arg(ap, int);
                                        if(n < 0)
                                        {
                                            *str = '-';
                                            str++;
                                            n = -n;
                                        }    
                                        itoa(n, buf);
                                        len = strlen(buf);
                                        if(len >= num)
                                        {
                                                memcpy(str, buf, strlen(buf));
                                                str += strlen(buf);
                                        }
                                        else
                                        {
                                                memset(str, '0', num-len);
                                                str += num-len;
                                                memcpy(str, buf, strlen(buf));
                                                str += strlen(buf);
                                        }
                                        break;
                                }    
                                case 'x': //16进制
                                {
                                        n = va_arg(ap, int);
                                        xtoa(n, buf);
                                        len = strlen(buf);
                                        if(len >= num)
                                        {
                                                memcpy(str, buf, len);
                                                str += len;
                                        }            
                                        else
                                        {
                                                memset(str, '0', num-len);
                                                str += num-len;
                                                memcpy(str, buf, len);
                                                str += len;
                                        }
                                        break;
                                }
                                case 's': //字符串
                                {
                                        s = va_arg(ap, char *);
                                        len = strlen(s);
                                        if(len >= num)
                                        {
                                                memcpy(str, s, strlen(s));
                                                str += strlen(s);
                                        }
                                        else
                                        {
                                                memset(str, '0', num-len);
                                                str += num-len;
                                                memcpy(str, s, strlen(s));
                                                str += strlen(s);
                                        }
                                        break;
                                }
                                default:
                                        break;
                        }
                }*/
                default:
                        break;
            }
        }
        else
        {
            *str = *fmt;
            str++;
            
            if(*fmt == '\n')
            {
                    
            }
        }
        fmt++;
    }
    *str = '\0';

    return str - out - 1;
}
/*
*功能：整型(int) 转化成 字符型(char)
*注意：不用 % / 符号的话，只能正确打印:0...9的数字对应的字符'0'...'9'
*/

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
