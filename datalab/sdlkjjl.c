#include<stdio.h>
int main(){  
  int x=0xd5555555;
    int y =x+1+~(x&(x<<1)),a=0x55+(0x55<<8),b=0x33+(0x33<<8),c=0x0f+(0x0f<<8);
  a=(a<<16)+a;b=(b<<16)+b;c=(c<<16)+c;
  printf("%x\n",y);
  y=(y&a)+((y>>1)&a);
  printf("%x\n",y);
  y=(y&b)+((y>>2)&b);
  printf("%x\n",y);
  y=(y&c)+((y>>4)&c);  
  printf("%x\n",y);
  printf("%d\n",(y+(y>>8)+(y>>16)+(y>>24))&0xff);
    }