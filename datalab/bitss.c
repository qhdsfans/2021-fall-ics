/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
/* 
 * bitNot - ~x without using ~
 *   Example: bitNot(0) = 0xffffffff
 *   Legal ops: ! & ^ | + << >>
 *   Max ops: 12
 *   Rating: 1
 */
int bitNot(int x) {
/* 
 * return x^(-1)
 */

  int r1=1<<31;
  int r2=r1>>31;
  return x^r2;
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {

/*
 * using x^y==(~x&y)|(x&~y),represent | with De Morgan's laws.
 */

  return ~(x&y)&~((~x)&(~y));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {

/*
 * if x is all odd-numbered ,x&0xaaaaaaaa==0xaaaaaaaa
 */

  int r1=0xaa+(0xaa<<8);
  int r2=r1+(r1<<16);
  return !((x&r2)^r2);
}
/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {

/*
 * shift x to the right by n,and shift x to the left by 32-n,then "add" them,considering the sign
 */
  int left,right,sign;
  sign=x>>31;
  left=x>>n;
  right=(x^sign)<<(31^n)<<1;
  return left^right;
}
/* 
 * palindrome - return 1 if x is palindrome in binary form,
 *   return 0 otherwise
 *   A number is palindrome if it is the same when reversed
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   Example: palindrome(0xff0000ff) = 1,
 *            palindrome(0xff00ff00) = 0
 *   Legal ops: ~ ! | ^ & << >> +
 *   Max ops: 40
 *   Rating: 4

 */
int palindrome(int x) {

/*
 * judging x==reverse(x),using effcient ways,only considering 16 bits
 */

    int m1 = 0x55555555;
    int m2 = 0x33333333;
    int m4 = 0x0f0f0f0f;

    int r = x;
    r = ((r & m1) << 1) | ((r >> 1) & m1);
    r = ((r & m2) << 2) | ((r >> 2) & m2);
    r = ((r & m4) << 4) | ((r >> 4) & m4);
    r = ((r)<<24)|((r&0xff00)<<8);

    x = x & 0xffff0000;
    return !(r ^ x);
}
/*
 * countConsecutive1 - return the number of consecutive 1‘s
 *   in the binary form of x
 *   Examples: countConsecutive1(0xff00ff00) = 2,
 *             countConsecutive1(0xf070f070) = 4,
 *             countConsecutive1(0b00101010001111110101110101110101) = 10
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int countConsecutive1(int x) {

/*
 *compute x^(x<<1) to simplify,then calculate with SWAR algorithm
 */
 
    int r = x ^ (x << 1);
    int m1 = 0x55 + (0x55 << 8);
    int m2 = 0x33 + (0x33 << 8);
    int m4 = 0x0f + (0x0f << 8);
    int m = 0xff;

    m1 = m1 + (m1 << 16);
    m2 = m2 + (m2 << 16);
    m4 = m4 + (m4 << 16);

    r = (r & m1) + ((r >> 1) & m1);
    r = (r & m2) + ((r >> 2) & m2);
    r = (r + (r >> 4)) & m4;
    r=(r+(r>>8)+(r>>16)+(r>>24))&m;
    return (1 + r) >> 1;
}
/* 
 * counter1To5 - return 1 + x if x < 5, return 1 otherwise, we ensure that 1<=x<=5
 *   Examples counter1To5(2) = 3,  counter1To5(5) = 1
 *   Legal ops: ~ & ! | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int counter1To5(int x) {

/*
 * get this 90 with searching algorithm,90>>x to make some bits correct,&x to get the ans of (x%5)
 * the searcher is showed below
 *
 * int a[]{ 1,2,3,4,5 };
 *  for (int i = 0; i < 256; ++i) {
 *      int f = 1;
 *      for (int j = 0; j < 4; ++j) {
 *          if (((i >> a[j]) & a[j]) != a[j])f = 0;
 *      }
 *      if (((i >> 5) & 5) != 0)f = 0;
 *      if (f)cout << i << endl;
 *  }
 * the output is 90,91,94,95
 */ 

  return ((90>>x)&x)+1;
}
/*
 * fullSub - 4-bits sub using bit-wise operations only.
 *   (0 <= x, y < 16) 
 *   Example: fullSub(12, 7) = 0x5,
 *            fullSub(7, 8) = 0xf,
 *   Legal ops: ~ | ^ & << >>
 *   Max ops: 35
 *   Rating: 2
 */
int fullSub(int x, int y) {

/*
 * a bit subtraction
 * repeat  {
 *          b = a & b;
 *          b = b << 1;
 *          a = a ^ b;
 *         }
 * until get the correct ans,then &15 to make the ans in range (0x0,0x10)
 */

    int a, b;
    a = x ^ y;

    b = a & y;
    b = b << 1;
    a = a ^ b;

    b = a & b;
    b = b << 1;
    a = a ^ b;

    b = a & b;
    b = b << 1;
    a = a ^ b;

    a = a & 15;
    return a;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {

/*
 * (x^y)>>31 to judge the sign,if same,compute the sign of x+~y,else compute the sign of x
 * notice x<=y <=> x-y<1 <=> x+~y<0
 */

  int sign=(x^y)>>31;
  int t=~(sign|y);        //if sign==0 calculate x+~y,else calculate the sign of x
  return ((x+t)>>31)&1;
}
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {

/*
 * if positive,return x,if negative,return ~x+1+tmin,notice when x=-0,return 0
 */   
  

  int sign = x >> 31;
  int n=1+(1<<31);
  return (x ^ sign) + (n & sign);
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {

/*
 * compute x+y,if overflow,return (ans>>31)+(of<<31),else return ans
 */

  int ans=x+y;
  int of=((x^ans)&(y^ans))>>31;//of<<31=tmin,if positive overflow,ans>>31=0;if negtive overflow,ans>>31=-1,-1+tmin=tmax
  return (ans>>(of&31))+(of<<31);
}
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x)
{

/* 
 * firstly solve (x/8)*5,then compute ((x%8)*5)/8,add them to get the ans
 */
 
  int rem,quo;
  int y=x&7;
  int sign=x>>31; 
  x=x>>3;
  quo=x+(x<<2);
  rem=((y<<2)+y+(sign&7))>>3;
  return quo+rem;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {

/* 
 * if NAN or INF return uf
 * if denorm frac*=2
 * if norm exp+=1,when overflow return INF
 */


  int exp=uf&0x7f800000;
  int sign=uf&0x80000000;
  switch(exp){
    case 0:return (uf<<1)|sign;
    case 0x7f800000:return uf;
    default:{
      uf+=0x00800000;
      switch(exp){
      case 0x7f000000:return uf&0xff800000;
      default:return uf;
      }
      }
  }

}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {

/*
 * if NAN or INF return uf
 * if norm exp-=1 if denorm frac/=2
 * using uf&3 to do the rounding
 */

  /*int exp=uf&0x7f800000;
  int sign=uf&0x80000000;
  int flag=((uf&3)==3);
  if(exp==0x7f800000)return uf;
  else if(exp<=0x800000)return sign|(((uf^sign)+flag)>>1);
  else return uf-0x800000;*/

  unsigned exp=uf&0xff800000;
  unsigned frac;
  unsigned shift=1;
  unsigned mask=0x7fffff;
  switch(exp){
    case 0x7f800000:
    case 0xff800000:
      return uf;
    case 0:
    case 0x80000000:
      break;
    default:
      exp-=0x800000;
      switch(exp){
        case 0:
        case 0x80000000:
          mask=0xffffff;
          break;
        default:
          shift=0;
      }
  }
  frac=(uf&mask)>>shift;
  switch(uf&3){
    case 3:frac+=shift;
  }
  return exp|frac;
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {

/* 
 * abs=(1+(frac>>23))<<(exp-127),change the order to handle rounding
 * notice all the denorm is switched to 0
 * using switch,just for fun
 */

    int sign = uf >> 31;
    int exp = (uf & 0x7f800000);
    int frac = uf & 0x7fffff;
    int abs = 0x800000 + frac;
    int shift = 0;
    switch (exp) {
    case 1065353216:shift = 23; break;
    case 1073741824:shift = 22; break;
    case 1082130432:shift = 21; break;
    case 1090519040:shift = 20; break;
    case 1098907648:shift = 19; break;
    case 1107296256:shift = 18; break;
    case 1115684864:shift = 17; break;
    case 1124073472:shift = 16; break;
    case 1132462080:shift = 15; break;
    case 1140850688:shift = 14; break;
    case 1149239296:shift = 13; break;
    case 1157627904:shift = 12; break;
    case 1166016512:shift = 11; break;
    case 1174405120:shift = 10; break;
    case 1182793728:shift = 9; break;
    case 1191182336:shift = 8; break;
    case 1199570944:shift = 7; break;
    case 1207959552:shift = 6; break;
    case 1216348160:shift = 5; break;
    case 1224736768:shift = 4; break;
    case 1233125376:shift = 3; break;
    case 1241513984:shift = 2; break;
    case 1249902592:shift = 1; break;
    case 1258291200:shift = 0; break;
    case 1266679808:shift = 1; break;
    case 1275068416:shift = 2; break;
    case 1283457024:shift = 3; break;
    case 1291845632:shift = 4; break;
    case 1300234240:shift = 5; break;
    case 1308622848:shift = 6; break;
    case 1317011456:shift = 7; break;
    default:
        if (exp < 1065353216)return 0;
        else return 0x80000000;
    }

    switch(exp){
        case 1266679808:
        case 1275068416:
        case 1283457024:
        case 1291845632:
        case 1300234240:
        case 1308622848:
        case 1317011456:abs<<=shift;break;
        default: abs>>=shift;break;
    }

    switch(sign){
      case 0:return abs;
      default:return -abs; 
    }
}
/* 
 * float_pwr2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned float_pwr2(int x) {

/*
 * get this switch with cpp,just for fun
 */
    /*if (x<-149)return 0;
    if (x < -126)return 1 << (x + 149);
    if (x < 128)return (x + 127) << 23;
    else return 255 << 23;*/
  switch(x){
        case 0xffffff6b:return 1;
        case 0xffffff6c:return 2;
        case 0xffffff6d:return 4;
        case 0xffffff6e:return 8;
        case 0xffffff6f:return 16;
        case 0xffffff70:return 32;
        case 0xffffff71:return 64;
        case 0xffffff72:return 128;
        case 0xffffff73:return 256;
        case 0xffffff74:return 512;
        case 0xffffff75:return 1024;
        case 0xffffff76:return 2048;
        case 0xffffff77:return 4096;
        case 0xffffff78:return 8192;
        case 0xffffff79:return 16384;
        case 0xffffff7a:return 32768;
        case 0xffffff7b:return 65536;
        case 0xffffff7c:return 131072;
        case 0xffffff7d:return 262144;
        case 0xffffff7e:return 524288;
        case 0xffffff7f:return 1048576;
        case 0xffffff80:return 2097152;
        case 0xffffff81:return 4194304;
        case 0xffffff82:return 8388608;
        case 0xffffff83:return 16777216;
        case 0xffffff84:return 25165824;
        case 0xffffff85:return 33554432;
        case 0xffffff86:return 41943040;
        case 0xffffff87:return 50331648;
        case 0xffffff88:return 58720256;
        case 0xffffff89:return 67108864;
        case 0xffffff8a:return 75497472;
        case 0xffffff8b:return 83886080;
        case 0xffffff8c:return 92274688;
        case 0xffffff8d:return 100663296;
        case 0xffffff8e:return 109051904;
        case 0xffffff8f:return 117440512;
        case 0xffffff90:return 125829120;
        case 0xffffff91:return 134217728;
        case 0xffffff92:return 142606336;
        case 0xffffff93:return 150994944;
        case 0xffffff94:return 159383552;
        case 0xffffff95:return 167772160;
        case 0xffffff96:return 176160768;
        case 0xffffff97:return 184549376;
        case 0xffffff98:return 192937984;
        case 0xffffff99:return 201326592;
        case 0xffffff9a:return 209715200;
        case 0xffffff9b:return 218103808;
        case 0xffffff9c:return 226492416;
        case 0xffffff9d:return 234881024;
        case 0xffffff9e:return 243269632;
        case 0xffffff9f:return 251658240;
        case 0xffffffa0:return 260046848;
        case 0xffffffa1:return 268435456;
        case 0xffffffa2:return 276824064;
        case 0xffffffa3:return 285212672;
        case 0xffffffa4:return 293601280;
        case 0xffffffa5:return 301989888;
        case 0xffffffa6:return 310378496;
        case 0xffffffa7:return 318767104;
        case 0xffffffa8:return 327155712;
        case 0xffffffa9:return 335544320;
        case 0xffffffaa:return 343932928;
        case 0xffffffab:return 352321536;
        case 0xffffffac:return 360710144;
        case 0xffffffad:return 369098752;
        case 0xffffffae:return 377487360;
        case 0xffffffaf:return 385875968;
        case 0xffffffb0:return 394264576;
        case 0xffffffb1:return 402653184;
        case 0xffffffb2:return 411041792;
        case 0xffffffb3:return 419430400;
        case 0xffffffb4:return 427819008;
        case 0xffffffb5:return 436207616;
        case 0xffffffb6:return 444596224;
        case 0xffffffb7:return 452984832;
        case 0xffffffb8:return 461373440;
        case 0xffffffb9:return 469762048;
        case 0xffffffba:return 478150656;
        case 0xffffffbb:return 486539264;
        case 0xffffffbc:return 494927872;
        case 0xffffffbd:return 503316480;
        case 0xffffffbe:return 511705088;
        case 0xffffffbf:return 520093696;
        case 0xffffffc0:return 528482304;
        case 0xffffffc1:return 536870912;
        case 0xffffffc2:return 545259520;
        case 0xffffffc3:return 553648128;
        case 0xffffffc4:return 562036736;
        case 0xffffffc5:return 570425344;
        case 0xffffffc6:return 578813952;
        case 0xffffffc7:return 587202560;
        case 0xffffffc8:return 595591168;
        case 0xffffffc9:return 603979776;
        case 0xffffffca:return 612368384;
        case 0xffffffcb:return 620756992;
        case 0xffffffcc:return 629145600;
        case 0xffffffcd:return 637534208;
        case 0xffffffce:return 645922816;
        case 0xffffffcf:return 654311424;
        case 0xffffffd0:return 662700032;
        case 0xffffffd1:return 671088640;
        case 0xffffffd2:return 679477248;
        case 0xffffffd3:return 687865856;
        case 0xffffffd4:return 696254464;
        case 0xffffffd5:return 704643072;
        case 0xffffffd6:return 713031680;
        case 0xffffffd7:return 721420288;
        case 0xffffffd8:return 729808896;
        case 0xffffffd9:return 738197504;
        case 0xffffffda:return 746586112;
        case 0xffffffdb:return 754974720;
        case 0xffffffdc:return 763363328;
        case 0xffffffdd:return 771751936;
        case 0xffffffde:return 780140544;
        case 0xffffffdf:return 788529152;
        case 0xffffffe0:return 796917760;
        case 0xffffffe1:return 805306368;
        case 0xffffffe2:return 813694976;
        case 0xffffffe3:return 822083584;
        case 0xffffffe4:return 830472192;
        case 0xffffffe5:return 838860800;
        case 0xffffffe6:return 847249408;
        case 0xffffffe7:return 855638016;
        case 0xffffffe8:return 864026624;
        case 0xffffffe9:return 872415232;
        case 0xffffffea:return 880803840;
        case 0xffffffeb:return 889192448;
        case 0xffffffec:return 897581056;
        case 0xffffffed:return 905969664;
        case 0xffffffee:return 914358272;
        case 0xffffffef:return 922746880;
        case 0xfffffff0:return 931135488;
        case 0xfffffff1:return 939524096;
        case 0xfffffff2:return 947912704;
        case 0xfffffff3:return 956301312;
        case 0xfffffff4:return 964689920;
        case 0xfffffff5:return 973078528;
        case 0xfffffff6:return 981467136;
        case 0xfffffff7:return 989855744;
        case 0xfffffff8:return 998244352;
        case 0xfffffff9:return 1006632960;
        case 0xfffffffa:return 1015021568;
        case 0xfffffffb:return 1023410176;
        case 0xfffffffc:return 1031798784;
        case 0xfffffffd:return 1040187392;
        case 0xfffffffe:return 1048576000;
        case 0xffffffff:return 1056964608;
        case 0x0:return 1065353216;
        case 0x1:return 1073741824;
        case 0x2:return 1082130432;
        case 0x3:return 1090519040;
        case 0x4:return 1098907648;
        case 0x5:return 1107296256;
        case 0x6:return 1115684864;
        case 0x7:return 1124073472;
        case 0x8:return 1132462080;
        case 0x9:return 1140850688;
        case 0xa:return 1149239296;
        case 0xb:return 1157627904;
        case 0xc:return 1166016512;
        case 0xd:return 1174405120;
        case 0xe:return 1182793728;
        case 0xf:return 1191182336;
        case 0x10:return 1199570944;
        case 0x11:return 1207959552;
        case 0x12:return 1216348160;
        case 0x13:return 1224736768;
        case 0x14:return 1233125376;
        case 0x15:return 1241513984;
        case 0x16:return 1249902592;
        case 0x17:return 1258291200;
        case 0x18:return 1266679808;
        case 0x19:return 1275068416;
        case 0x1a:return 1283457024;
        case 0x1b:return 1291845632;
        case 0x1c:return 1300234240;
        case 0x1d:return 1308622848;
        case 0x1e:return 1317011456;
        case 0x1f:return 1325400064;
        case 0x20:return 1333788672;
        case 0x21:return 1342177280;
        case 0x22:return 1350565888;
        case 0x23:return 1358954496;
        case 0x24:return 1367343104;
        case 0x25:return 1375731712;
        case 0x26:return 1384120320;
        case 0x27:return 1392508928;
        case 0x28:return 1400897536;
        case 0x29:return 1409286144;
        case 0x2a:return 1417674752;
        case 0x2b:return 1426063360;
        case 0x2c:return 1434451968;
        case 0x2d:return 1442840576;
        case 0x2e:return 1451229184;
        case 0x2f:return 1459617792;
        case 0x30:return 1468006400;
        case 0x31:return 1476395008;
        case 0x32:return 1484783616;
        case 0x33:return 1493172224;
        case 0x34:return 1501560832;
        case 0x35:return 1509949440;
        case 0x36:return 1518338048;
        case 0x37:return 1526726656;
        case 0x38:return 1535115264;
        case 0x39:return 1543503872;
        case 0x3a:return 1551892480;
        case 0x3b:return 1560281088;
        case 0x3c:return 1568669696;
        case 0x3d:return 1577058304;
        case 0x3e:return 1585446912;
        case 0x3f:return 1593835520;
        case 0x40:return 1602224128;
        case 0x41:return 1610612736;
        case 0x42:return 1619001344;
        case 0x43:return 1627389952;
        case 0x44:return 1635778560;
        case 0x45:return 1644167168;
        case 0x46:return 1652555776;
        case 0x47:return 1660944384;
        case 0x48:return 1669332992;
        case 0x49:return 1677721600;
        case 0x4a:return 1686110208;
        case 0x4b:return 1694498816;
        case 0x4c:return 1702887424;
        case 0x4d:return 1711276032;
        case 0x4e:return 1719664640;
        case 0x4f:return 1728053248;
        case 0x50:return 1736441856;
        case 0x51:return 1744830464;
        case 0x52:return 1753219072;
        case 0x53:return 1761607680;
        case 0x54:return 1769996288;
        case 0x55:return 1778384896;
        case 0x56:return 1786773504;
        case 0x57:return 1795162112;
        case 0x58:return 1803550720;
        case 0x59:return 1811939328;
        case 0x5a:return 1820327936;
        case 0x5b:return 1828716544;
        case 0x5c:return 1837105152;
        case 0x5d:return 1845493760;
        case 0x5e:return 1853882368;
        case 0x5f:return 1862270976;
        case 0x60:return 1870659584;
        case 0x61:return 1879048192;
        case 0x62:return 1887436800;
        case 0x63:return 1895825408;
        case 0x64:return 1904214016;
        case 0x65:return 1912602624;
        case 0x66:return 1920991232;
        case 0x67:return 1929379840;
        case 0x68:return 1937768448;
        case 0x69:return 1946157056;
        case 0x6a:return 1954545664;
        case 0x6b:return 1962934272;
        case 0x6c:return 1971322880;
        case 0x6d:return 1979711488;
        case 0x6e:return 1988100096;
        case 0x6f:return 1996488704;
        case 0x70:return 2004877312;
        case 0x71:return 2013265920;
        case 0x72:return 2021654528;
        case 0x73:return 2030043136;
        case 0x74:return 2038431744;
        case 0x75:return 2046820352;
        case 0x76:return 2055208960;
        case 0x77:return 2063597568;
        case 0x78:return 2071986176;
        case 0x79:return 2080374784;
        case 0x7a:return 2088763392;
        case 0x7b:return 2097152000;
        case 0x7c:return 2105540608;
        case 0x7d:return 2113929216;
        case 0x7e:return 2122317824;
        case 0x7f:return 2130706432;
        default:{
          if(x>=128)return 0x7f800000;
          else return 0;
        }
  }
}
