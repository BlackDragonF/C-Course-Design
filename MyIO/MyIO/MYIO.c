#include "MYIO.h"

/*判断一个字符是否为空字符*/
#define is_blank(ch) (((ch) == ' ') || ((ch) == '\n') || ((ch) == '\f') || ((ch) == '\r') || ((ch) == '\t') || ((ch) == '\v'))
/*判断一个字符是否为数字字符*/
#define is_num(ch) (((ch) >= '0') && ((ch) <= '9'))


/*************************************************
 Function: mygets
 Description: a replica for gets
 Input: char * indicating the pointer to the string
 Output: None
 Return: pointer to the string
 *************************************************/
char * mygets(char * str)
{
	char * rst = str;
	char ch;
	while ((ch = getchar()) != '\n')
	{
		*str = ch;
		str++;
	}
	*str = '\n';
	str++;
	*str = '\0';
	return rst;
}

/*************************************************
 Function: myputs
 Description: a replica for puts
 Input: const char * indicating the pointer to the string
 Output: the string that passes in with \n
 Return: None
 *************************************************/
void myputs(const char * str)
{
	while (*str != '\0')
	{
		putchar(*str);
		str++;
	}
	putchar('\n');
}

/*************************************************
 Function: putint
 Description: put the int number to stdout
 Input: int indicating the number
 Output: the number that passes in
 Return: void
 *************************************************/
void putint(int num)
{
    if (num < 0) num = -num;
	if (num == 0) return;
	putint(num / 10);
	putchar((num % 10) + '0');
}

/*************************************************
 Function: putdec
 Description: put the decimal number to stdout
 Input: double indicating the number
 Output: the decimal that passes in
 Return: void
 *************************************************/
void putdec(double num)
{
	int count;
	if (num < 0) num = -num;
	for (count = 0 ; count < 6 ; count++)
	{
		num *= 10;
		putchar (((int)num % 10) + '0');
	}
}

/*************************************************
 Function: myprintf
 Description: a low replica for printf
 Input: char * indicating the format string, and the flexible arguments.
 Output: the format string that passes in
 Return: None
 *************************************************/
void myprintf(const char * format, ...)
{
	va_list ap;
	va_start(ap, format);

	while (*format != '\0')
	{
		if (*format != '%')
			putchar(*format);
		else if (*(++format) == 'c')
			putchar(va_arg(ap, int));
		else if (*(format) == 's')
		{
			char * str = va_arg(ap, char *);
			while (*str != '\0')
			{
				putchar(*str);
				str++;
			}
		}
		else if (*(format) == 'd')
		{
			int num = va_arg(ap, int);
			if (num < 0) putchar('-');
            if (num != 0)
                putint(num);
            else
                putchar('0');
		}
		else if (*(format) == 'f')
		{
			double num = va_arg(ap, double);
			if (num < 0) putchar('-');
            if ((int)(num) != 0)
                putint(num);
            else
                putchar('0');
			putchar('.');
			putdec((int)num - num);
		}
        else if (*(format++) == 'L')
        {
            if (*(format) == 'f')
            {
                long double num = va_arg(ap, long double);
                if (num < 0) putchar('-');
                if ((int)(num) != 0)
                    putint(num);
                else
                    putchar('0');
                putchar('.');
                putdec((int)num - num);
            }
            else
            {
                putchar('%');
                putchar('l');
                putchar(*format);
            }
        }
		else
		{
			putchar('%');
			putchar(*format);
		}
		format++;
	}
	va_end(ap);
}

/*************************************************
 Function: myscanf
 Description: a low replica for scanf
 Input: const char * indicating the pointer to the format string, and the flexible arguments.
 Output: None
 Return: None
 *************************************************/
void myscanf(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    char ch;
    while (*format != '\0')
    {
        if (*(format) == '%')
        {
            if (*(++format) == 'c')
            {
                char * pch = va_arg(ap, char *);
                *pch = getchar();
            }
            else if (*(format) == 's')
            {
                char * str = va_arg(ap, char *);
                while ((ch = getchar()) && is_blank(ch));
                do
                {
                    *str = ch;
                    str++;
                } while ((ch = getchar()) && !is_blank(ch));
                *str = '\0';
                ungetc(ch, stdin);
            }
            else if (*(format) == 'd')
            {
                int * pint = va_arg(ap, int *);
                *pint = 0;
                while ((ch = getchar()) && is_blank(ch));
                int sign = 1;
                if (ch == '-')
                    sign = -1;
                else if (is_num(ch))
                    *pint += ch - '0';
                while ((ch = getchar()) && is_num(ch))
                    *pint = *pint * 10 + ch - '0';
                *pint *= sign;
                ungetc(ch, stdin);
            }
            else if (*(format) == 'f')
            {
                float * pfloat = va_arg(ap, float *);
                *pfloat = 0;
                while ((ch = getchar()) && is_blank(ch));
                int sign = 1;
                if (ch == '-')
                    sign = -1;
                else if (is_num(ch))
                    *pfloat += ch - '0';
                while ((ch = getchar()) && is_num(ch))
                    *pfloat = *pfloat * 10 + ch - '0';
                if (ch == '.')
                {
                    int count = 10;
                    while((ch = getchar()) && is_num(ch))
                    {
                        *pfloat += (float)(ch - '0') / count;
                        count *= 10;
                    }
                }
                *pfloat *= sign;
                ungetc(ch, stdin);
            }
            else if (*(format++) == 'l')
                if (*(format) == 'f')
                {
                    double * pdouble = va_arg(ap, double *);
                    *pdouble = 0;
                    while ((ch = getchar()) && is_blank(ch));
                    int sign = 1;
                    if (ch == '-')
                        sign = -1;
                    else if (is_num(ch))
                        *pdouble += ch - '0';
                    while ((ch = getchar()) && is_num(ch))
                        *pdouble = *pdouble * 10 + ch - '0';
                    if (ch == '.')
                    {
                        int count = 10;
                        while((ch = getchar()) && is_num(ch))
                        {
                            *pdouble += (double)(ch - '0') / count;
                            count *= 10;
                        }
                    }
                    *pdouble *= sign;
                    ungetc(ch, stdin);
                }
        }
        format++;
    }
}
