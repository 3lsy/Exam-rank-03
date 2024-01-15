#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return i;
}

int	ft_putstr(char *str)
{
	if (!str)
		return (write(1, "(null)", 6));
	return (write(1, str, ft_strlen(str)));
}

int	ft_putnbr(int nb)
{
	int i = 0;
	char res[42];
	int written = 0;
	char *base = "0123456789";
	long n;

	n = nb;
	if (n == 0)
		return (write(1, "0", 1));
	else if (n < 0){
		write(1, "-", 1);
		written++;
		n *= -1;
	}
	while (n) {
		res[i] = base[n % 10];
		n = n / 10;
		i++;
	}
	written += i;
	while (i != 0){
		i--;
		write(1, &res[i], 1);
	}
	return written;
}

unsigned long	ft_puthex(unsigned long n)
{
	unsigned long i = 0;
	char res[42];
	unsigned long written = 0;
	char *base = "0123456789abcdef";

	if (n == 0)
		return (write(1, "0", 1));
	while (n) {
		res[i] = base[n % 16];
		n = n / 16;
		i++;
	}
	written = i;
	while (i != 0){
		i--;
		write(1, &res[i], 1);
	}
	return written;
}

int	ft_conv(va_list ap, char conv)
{
	int counter = 0;

	if (conv == 's')
		counter += ft_putstr(va_arg(ap, char *));
	else if (conv == 'd')
		counter += ft_putnbr(va_arg(ap, int));
	else if (conv == 'x')
		counter += ft_puthex(va_arg(ap, unsigned int));
	return (counter);
}

int	v_printf(va_list ap, const char *format)
{
	int i = 0;
	int counter = 0;

	while (format[i])
	{
		//check if valid conversion
		if (format[i] == '%' && ((format[i+1] == 's') ||
		(format[i+1] == 'd') || (format[i+1] == 'x'))) {
			counter += ft_conv(ap, format[i+1]);
			i++;
		}
		//check if backslash
		else if (format[i] == '\\' && format[i+1]) {
			counter += write(1, &format[i], 2);
			i++;
		}
		//write the other characters
		else {
			counter += write(1, &format[i], 1);
		}
		i++;
	}
	return counter;
}

int	ft_printf(const char *format, ... )
{
	int	counter = 0;
	va_list ap;

	// start args
	va_start(ap, format);
	// call vprintf
	counter = v_printf(ap, format);
	// end args
	va_end(ap);
	return (counter);
}

//main
int main(void)
{
	int out1;
	int out2;

	out1 = ft_printf("Hello %s\n", "toto");
	out2 = printf("Hello %s\n", "toto");
	printf("(elsy's out) %d\n(printf's out) %d\n", out1, out2);

	out1 = ft_printf("Magic %s is %d\n", "number", 42);
	out2 = printf("Magic %s is %d\n", "number", 42);
	printf("(elsy's out) %d\n(printf's out) %d\n", out1, out2);

	out1 = ft_printf("Hexadecimal for %d is %x\n", 42, 42);
	out2 = printf("Hexadecimal for %d is %x\n", 42, 42);
	printf("(elsy's out) %d\n(printf's out) %d\n", out1, out2);
	return 0;
}