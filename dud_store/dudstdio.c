
#include <stdint.h>
#include "console.h"
#include "dudstdio.h"

# define do_div(n, base) ({						\
	unsigned int __base = (base);					\
	unsigned int __rem;						\
	__rem = ((unsigned long long)(n)) % __base;			\
	(n) = ((unsigned long long)(n)) / __base;			\
	__rem;								\
})

void dud_number(unsigned long long num, int base)
{
	unsigned long long orignum;
	char tmp[86];
	int i;

	// tmp[0] = 0x39;
	// tmp[1] = 0x38;
	// tmp[2] = 0x0;
	// puts(tmp);

	i = 0;
	orignum = num;
	while (num != 0) {
		unsigned int rem = do_div(num, base);
		tmp[i++] = rem;
	}
	printhex("dud_number1", tmp, i);

	num = orignum;
	i = 0;
	while (num != 0) {
		unsigned int rem = do_div(num, base);
		tmp[i++] = rem;
	}
	printhex("dud_number2", tmp, i);
	printhex("^ base2", &base, sizeof(base));
	printhex("^ num2", &orignum, sizeof(orignum));
}

void printhex(const char *msg, const void* buf, long len) {
	puts(msg);
	puts("\n");
	long d = 32768;
	if (len < d) {
		d = len;
	}
	const uint8_t *h = buf;
	const char *digs = "0123456789abcdef";
	for (long i = 0; i < d; i++) {
		char c[4];
		c[0] = digs[(h[i] >> 4)];
		c[1] = digs[(h[i] & 0xf)];
		c[2] = ' ';
		c[3] = 0x0;
		puts(c);
		if ((i % 32) == 31) {
			puts("\n");
		}
	}
	puts("_\n");
}

// int dud_vsprintf(char *buf, const char *fmt, va_list args)
// {
// 	int len;
// 	unsigned long long num;
// 	int i, base;
// 	char * str;
// 	const char *s;

// 	int flags;		/* flags to number() */

// 	int field_width;	/* width of output field */
// 	int precision;		/* min. # of digits for integers; max
// 				   number of chars for from string */
// 	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
// 	                        /* 'z' support added 23/7/1999 S.H.    */
// 				/* 'z' changed to 'Z' --davidm 1/25/99 */

	
// 	for (str=buf ; *fmt ; ++fmt) {
// 		if (*fmt != '%') {
// 			*str++ = *fmt;
// 			continue;
// 		}
			
// 		/* process flags */
// 		flags = 0;
// 		repeat:
// 			++fmt;		/* this also skips first '%' */
// 			switch (*fmt) {
// 				case '-': flags |= LEFT; goto repeat;
// 				case '+': flags |= PLUS; goto repeat;
// 				case ' ': flags |= SPACE; goto repeat;
// 				case '#': flags |= SPECIAL; goto repeat;
// 				case '0': flags |= ZEROPAD; goto repeat;
// 				}
		
// 		/* get field width */
// 		field_width = -1;
// 		if ('0' <= *fmt && *fmt <= '9')
// 			field_width = skip_atoi(&fmt);
// 		else if (*fmt == '*') {
// 			++fmt;
// 			/* it's the next argument */
// 			field_width = va_arg(args, int);
// 			if (field_width < 0) {
// 				field_width = -field_width;
// 				flags |= LEFT;
// 			}
// 		}

// 		/* get the precision */
// 		precision = -1;
// 		if (*fmt == '.') {
// 			++fmt;	
// 			if ('0' <= *fmt && *fmt <= '9')
// 				precision = skip_atoi(&fmt);
// 			else if (*fmt == '*') {
// 				++fmt;
// 				/* it's the next argument */
// 				precision = va_arg(args, int);
// 			}
// 			if (precision < 0)
// 				precision = 0;
// 		}

// 		/* get the conversion qualifier */
// 		qualifier = -1;
// 		if (*fmt == 'l' && *(fmt + 1) == 'l') {
// 			qualifier = 'q';
// 			fmt += 2;
// 		} else if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L'
// 			|| *fmt == 'Z') {
// 			qualifier = *fmt;
// 			++fmt;
// 		}

// 		/* default base */
// 		base = 10;

// 		switch (*fmt) {
// 		case 'c':
// 			if (!(flags & LEFT))
// 				while (--field_width > 0)
// 					*str++ = ' ';
// 			*str++ = (unsigned char) va_arg(args, int);
// 			while (--field_width > 0)
// 				*str++ = ' ';
// 			continue;

// 		case 's':
// 			s = va_arg(args, char *);
// 			if (!s)
// 				s = "<NULL>";

// 			len = strnlen(s, precision);

// 			if (!(flags & LEFT))
// 				while (len < field_width--)
// 					*str++ = ' ';
// 			for (i = 0; i < len; ++i)
// 				*str++ = *s++;
// 			while (len < field_width--)
// 				*str++ = ' ';
// 			continue;

// 		case 'p':
// 			if (field_width == -1) {
// 				field_width = 2*sizeof(void *);
// 				flags |= ZEROPAD;
// 			}
// 			// str = dud_number(str,
// 			// 	(unsigned long) va_arg(args, void *), 16,
// 			// 	field_width, precision, flags);
// 			continue;


// 		case 'n':
// 			if (qualifier == 'l') {
// 				long * ip = va_arg(args, long *);
// 				*ip = (str - buf);
// 			} else if (qualifier == 'Z') {
// 				size_t * ip = va_arg(args, size_t *);
// 				*ip = (str - buf);
// 			} else {
// 				int * ip = va_arg(args, int *);
// 				*ip = (str - buf);
// 			}
// 			continue;

// 		case '%':
// 			*str++ = '%';
// 			continue;

// 		/* integer number formats - set up the flags and "break" */
// 		case 'o':
// 			base = 8;
// 			break;

// 		case 'X':
// 			flags |= LARGE;
// 		case 'x':
// 			base = 16;
// 			break;

// 		case 'd':
// 		case 'i':
// 			flags |= SIGN;
// 		case 'u':
// 			break;

// 		default:
// 			*str++ = '%';
// 			if (*fmt)
// 				*str++ = *fmt;
// 			else
// 				--fmt;
// 			continue;
// 		}
// 		if (qualifier == 'l') {
// 			num = va_arg(args, unsigned long);
// 			if (flags & SIGN)
// 				num = (signed long) num;
// 		} else if (qualifier == 'q') {
// 			num = va_arg(args, unsigned long long);
// 			if (flags & SIGN)
// 				num = (signed long long) num;
// 		} else if (qualifier == 'Z') {
// 			num = va_arg(args, size_t);
// 		} else if (qualifier == 'h') {
// 			num = (unsigned short) va_arg(args, int);
// 			if (flags & SIGN)
// 				num = (signed short) num;
// 		} else {
// 			num = va_arg(args, unsigned int);
// 			if (flags & SIGN)
// 				num = (signed int) num;
// 		}
// 		dud_number(num, base);
// 	}
// 	*str = '\0';
// 	return str-buf;
// }

// int
// dud_printf(const char *fmt, ...)
// {
// 	va_list args;
// 	int n;

// 	va_start(args, fmt);
// 	n = dud_vsprintf(sprint_buf, fmt, args);
// 	va_end(args);
// 	return n;
// }
