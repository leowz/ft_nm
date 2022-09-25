#include "ft_nm.h"

static char			if_upper(char c, uint8_t ntype)
{
	if (ntype & N_EXT)
		return (ft_toupper(c));
	if (c == 'c' || c == 'u')
		return ('?');
	return (c);
}

static char			find_sect_type(t_sym sym)
{
	if (ft_strequ(sym.segname, "__TEXT") && ft_strequ(sym.sectname, "__text"))
		return (if_upper('t', sym.ntype));
	if (ft_strequ(sym.segname, "__DATA") && ft_strequ(sym.sectname, "__data"))
		return (if_upper('d', sym.ntype));
	if (ft_strequ(sym.segname, "__DATA") && ft_strequ(sym.sectname, "__bss"))
		return (if_upper('b', sym.ntype));
	return (if_upper('s', sym.ntype));
}

char				get_char_type(t_sym sym)
{
	int		type;

	type = sym.ntype & N_TYPE;
	if ((type == N_UNDF || type == N_PBUD) && sym.nsect == NO_SECT)
	{
		if (sym.value && type != N_PBUD)
			return (if_upper('c', sym.ntype));
		return (if_upper('u', sym.ntype));
	}
	else if (type == N_ABS)
		return (if_upper('a', sym.ntype));
	else if (type == N_SECT && sym.nsect != NO_SECT && sym.nsect <= MAX_SECT)
		return (find_sect_type(sym));
	else if (type == N_INDR)
		return (if_upper('i', sym.ntype));
	return (if_upper('?', sym.ntype));
}

