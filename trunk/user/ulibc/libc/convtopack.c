#include <convtopack.h>
#include <sys/io.h>
#include <string.h>
#include <stdlib.h>


//ltoa implementation **************
#define INT_DIGITOS 63
static char buf[INT_DIGITOS + 2];
/* Buffer para INT_DIGITS dígitos, signo - y fin de string '\0' */

char *ltoa(long int i, unsigned int base) {
   char *p = buf + INT_DIGITOS + 1;                   /* apunta a posición de unidades */
   int dig, signo=0;
   if (i<0) {
      signo=1;i = -i;
   }
   if(base<=0 || base>36) base=10;          /*evita división por cero */
   do {
       dig=(i%base);
       if (dig <=9) *--p = '0' + dig; 
         else *--p= '7'+ dig ; 
         i /= base;
   }
   while (i != 0);
   if(signo) *--p = '-';
   return p;
}
//End ltoa implementation **************

/* reverse:  reverse string s in place */
void strreverse(char* begin, char* end) {
	char aux;

	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}
	
void itoa(int value, char* str, int base) {
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	char* wstr=str;

	int sign;

	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return; }
	// Take care of sign
	if ((sign=value) < 0) value = -value;
	// Conversion. Number is reversed.
	do *wstr++ = num[value%base]; while(value/=base);
	if(sign<0) *wstr++='-';
	*wstr='\0';
	// Reverse string

	strreverse(str,wstr-1);
}


//dtoa implementation **************
char * dtoa (double value, int ndigits, char *buf)
{
  char *p = buf;
  sprintf (buf, "%-#.*g", ndigits, value);
  /* It seems they expect us to return .XXXX instead of 0.XXXX */ 
  if (*p == '-')
    p++;
  if (*p == '0' && p[1] == '.')
    memmove (p, p + 1, strlen (p + 1) + 1);
  /* They want Xe-YY, not X.e-YY, and XXXX instead of XXXX.  */
  p = strchr (buf, 'e');
  if (!p)
    {
      p = buf + strlen (buf);
      /* They don't want trailing zeroes.  */
      while (p[-1] == '0' && p > buf + 2)
        *--p = '\0';
    }
  if (p > buf && p[-1] == '.')
    memmove (p - 1, p, strlen (p) + 1);
  //printf("El bufer es: %s\n\n",buf);
  return buf;
}
//End dtoa implementation **************

//ftoa implementation **************
char *ftoa(float f, int ndigits){
   char * buffer;
   buffer=malloc(30);
     return(dtoa((double)f,ndigits,buffer));
}
//end ftoa implementation **************


char *FloatMsgtoPack(float float_subMsgtoPack, int ndigits){
  ftoa(float_subMsgtoPack, ndigits);
}

char* IntMsgtoPack(int int_subMsgtoPack){
  char* MsgtoSend;
  MsgtoSend=malloc(10);

  itoa(int_subMsgtoPack,MsgtoSend,10);

  return MsgtoSend;
}

char * DoubleMsgtoPack(double double_subMsgtoPack, int ndigits){
  char *buffer;
  //MsgtoSend=malloc(LENGHTMSGTOPACK);
  buffer = malloc(15);
  dtoa(double_subMsgtoPack, ndigits, buffer);
}

char * LongIntMsgtoPack(long int longint_subMsgtoPack){
    return ltoa(longint_subMsgtoPack, 10);
}


char* StrMsgtoPack(char* char_subMsgtoPack){
  char* MsgtoSend;
  MsgtoSend=malloc(15);

  strcat(MsgtoSend,char_subMsgtoPack);

 return MsgtoSend;
}

char* substring(const char* cadena, int inicio, int chars){
    char* sub;
    int total = (inicio + chars - 2);
    int lenCad = strlen(cadena) - 1;
    if (total > lenCad){
        sub = (char *)malloc(lenCad * sizeof(char));
        sprintf(sub, "%.*s", lenCad, cadena + (inicio - 1) );
    }
    else{
        sub = (char *)malloc(total * sizeof(char));
        sprintf(sub, "%.*s", chars , cadena + (inicio - 1));
    }
     return sub;
}

// atof implementation ********************
double  atof(char* string)
{   
    float fin=0;
    double n=0, p=0;
    int i=0, s=0;

    for ( i=0; string[i]==' ' || string[i]=='\n' || string[i]=='\t'; i++ );

    if ( string[i]=='+' || string[i]=='-' )
        s=(string[i++]=='+') ? 1:-1;
    else
        s=1;
    for ( n=0 ; string[i]>='0' && string[i]<='9'; i++ )
        n = 10*n+string[i]-'0';

    if (string[i]=='.')
        i++;
    for ( p=1 ; string[i]>='0' && string[i]<='9'; i++ )
    {
        n = 10*n+string[i]-'0';
        p *= 10;
    }
free(string);
	fin=s*n/p;
    return fin;
}
// END atof implementation *********************************
