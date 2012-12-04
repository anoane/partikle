

#ifndef _CONVTOPACK_H_
#define _CONVTOPACK_H_

char *ltoa(long int i, unsigned int base);
void strreverse(char* begin, char* end);
void itoa(int value, char* str, int base);
char *dtoa (double value, int ndigits, char *buf);
char *ftoa(float f, int ndigits);
char *FloatMsgtoPack(float float_subMsgtoPack, int ndigits);
char *IntMsgtoPack(int int_subMsgtoPack);
char *DoubleMsgtoPack(double double_subMsgtoPack, int ndigits);
char *LongIntMsgtoPack(long int longint_subMsgtoPack);
char *StrMsgtoPack(char *char_subMsgtoPack);
char *substring(const char* cadena, int inicio, int chars);
double atof(char* string);

#endif


