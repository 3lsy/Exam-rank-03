## Subject

```
Assignment name  : ft_printf
Expected files   : ft_printf.c
Allowed functions: malloc, free, write, va_start, va_arg, va_copy, va_end
--------------------------------------------------------------------------------

Write a function named `ft_printf` that will mimic the real printf with the following constraints:

- It will manage only the following conversions: s,d, and x

Your function must be declared as follows:

int ft_printf(const char *, ... );

Before you start we advise you to read the `man 3 printf` and the `man va_arg`.
To test your program compare your results with the true printf.

Your function must not leak.


Exemples of the function output:

call: ft_printf("Hello %s\n", "toto");
out:Hello toto$

call: ft_printf("Magic %s is %d", "number", 42);
out:Magic number is 42%

call: ft_printf("Hexadecimal for %d is %x\n", 42, 42);
out:Hexadecimal for 42 is 2a$

Warning: Your function must not have any leaks, this will be checked during moulinette

--------------------------------------------------------------------------------
```

## Conversions

- s (string)
  - `ft_putstr()`
- d (decimal)
  - `ft_putnbr`
- x (hexadecimal)
  - `ft_puthex()`

## main
void	pconv(char conv, int fd, t_info	*x)
{
	int	*counter;

	counter = &x->counter;
	if (conv == 'd')
		*counter += ft_putnbr_base(va_arg(x->ap, int), DECIMAL, fd);
	else if (conv == '%')
		*counter += write(fd, "%", 1);
	else if (conv == 's')
		*counter += ft_putstr_fd(va_arg(x->ap, char *), fd);
	else if (conv == 'x')
		*counter += ft_uputnbr_base(va_arg(x->ap, unsigned int), LHEX, fd);
}

void	v_printf(const char *format, int fd, t_info *x)
{
	int	i;
	int	*counter;

	counter = &x->counter;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && ft_strchr(CONVERSIONS, format[i + 1]) != NULL)
			pconv(format[(i++) + 1], fd, x);
		else if (format[i] == '\\' && format[i + 1])
			*counter += write(fd, &format[i++], 2);
		else
			*counter += write(fd, &format[i], 1);
		i++;
	}
}

int	ft_printf(const char *format, ...)
{
	t_info	x;

	x.counter = 0;
	va_start(x.ap, format);
	v_printf(format, 1, &x);
	va_end(x.ap);
	return (x.counter);
}

## putstr

```C
int	ft_putstr(char *s)
{
	return (write(1, s, ft_strlen(s)));
}
```

## putnbr

```C
void	ft_putnbr(int n)
{
	if (n >= 10)
	{
		ft_putnbr((n / 10));
		ft_putchar((n % 10) + 48);
	}
	else if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648");
	}
	else if (n < 0)
	{
		n *= -1;
		ft_putchar('-');
		ft_putnbr(n);
	}
	else if (n < 10)
	{
		ft_putchar(n + 48);
	}
}
```

## puthex

```C
uintmax_t	ft_uputnbr_base(uintmax_t nbr, char *base, int fd)
{
	uintmax_t		i;
	uintmax_t		size;
	char			res[42];
	uintmax_t		written;

	if (nbr == 0)
		return (write(fd, "0", 1));
	size = ft_strlen(base);
	i = 0;
	while (nbr)
	{
		res[i++] = base[nbr % size];
		nbr = nbr / size;
	}
	written = i;
	while (i != 0)
		write(fd, &res[--i], 1);
	return (written);
}
```

## Syntax Token

```
%🗶
```
