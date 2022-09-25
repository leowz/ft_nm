#include "ft_nm.h"

void			print_value(t_sym sym, char c)
{
	if (sym.arch == ARCH_64)
	{
		if ((!(sym.ntype & N_EXT) || sym.value ||
						(sym.ntype & N_TYPE) == N_SECT || c == 'A') && c != 'I')
			ft_printf("%016llx", sym.value);
		else
			ft_putstr("                ");
	}
	else
	{
		if ((!(sym.ntype & N_EXT) || sym.value ||
								(sym.ntype & N_TYPE) == N_SECT || c == 'A'))
			ft_printf("%08llx", sym.value);
		else
			ft_putstr("        ");
	}
}

static void		print_ext(t_sym sym)
{
	if (sym.ntype & N_EXT)
		ft_putstr(" external ");
	else if (sym.ntype & N_PEXT)
		ft_putstr(" private external ");
	else
		ft_putstr(" non-external ");
}

void			print_m(t_sym sym)
{
	char	c;

	c = ft_toupper(get_char_type(sym));
	if ((sym.ntype & N_TYPE) == N_UNDF && sym.value)
		ft_putstr(" (common)");
	else if (c == 'U')
		ft_putstr(" (undefined)");
	else if (c == 'C')
		ft_putstr(" (common)");
	else if (c == 'A')
		ft_putstr(" (absolute)");
	else if (c == 'I')
		ft_putstr(" (indirect)");
	else if ((sym.ntype & N_TYPE) == N_SECT)
	{
		if (sym.sectname[0] && sym.segname[0])
			ft_printf(" (%s,%s)", sym.segname, sym.sectname);
		else
			ft_putstr(" (?,?)");
	}
	else
		ft_putstr(" (?)");
	print_ext(sym);
}

