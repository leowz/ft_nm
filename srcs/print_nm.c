#include "ft_nm.h"

static void		print_path(t_arch *input)
{
	ft_printf("%s: ", input->path);
}

static t_bool	print_cpu_type(t_arch *input)
{
	const char	*name;

	name = get_arch_info_from_cpu_type(input->cpu_type,
													input->cpu_subtype).name;
	if (name && !ft_strcmp(name, GET_ARCH))
		return (FALSE);
	if (!name)
	{
		ft_printf("\n%s (for architecture ):\n", input->path);
		return (FALSE);
	}
	ft_printf("\n%s (for architecture %s):\n", input->path, name);
	return (FALSE);
}

static void		print_nm_simple(t_arch *input, t_sym sym, char c)
{
/*	if (get_nm_flags()->a_up)
		print_path(input);
	if (!get_nm_flags()->j && !get_nm_flags()->u)
	{
		print_value(sym, c);
		if (!get_nm_flags()->m)
			ft_printf(" %c ", c);
		if (sym.ntype & N_STAB)
			ft_putstr(" DEBUG SYMBOL ");
		else if (get_nm_flags()->m)
			print_m(sym);
	}*/
	if (sym.name)
		ft_putstr(sym.name);
	else
		ft_putstr("bad string index");
	if (c == 'I')
		ft_printf(" (indirect for %s)", sym.name);
	ft_putchar('\n');
}

void			print_nm(t_arch *input)
{
	t_sym	sym;
	t_list	*lst;
	char	c;

	if (print_cpu_type(input))
		return ;
	lst = input->list;
	while (lst)
	{
		sym = *(t_sym *)lst->content;
		c = get_char_type(sym);
		print_nm_simple(input, sym, c);
		lst = lst->next;
	}
}

