#include "ft_nm.h"

int print_error(const char *pgm, int ret)
{
    ft_printf("%s\n", pgm);
    return ret;
}

int		return_error(const char *pgm, int ret)
{
	print_error(pgm, ret);
	return (ret);
}

void	*return_error_null(char *pgm, int type)
{
	print_error(pgm, type);
	return (NULL);
}

