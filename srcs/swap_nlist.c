#include "ft_nm.h"

void	swap_nlist(struct nlist *symbol)
{
	symbol->n_un.n_strx = ft_swap_int32(symbol->n_un.n_strx);
	symbol->n_desc = ft_swap_int16(symbol->n_desc);
	symbol->n_value = ft_swap_int32(symbol->n_value);
}

void	swap_nlist_64(struct nlist_64 *symbol)
{
	symbol->n_un.n_strx = ft_swap_int32(symbol->n_un.n_strx);
	symbol->n_desc = ft_swap_int16(symbol->n_desc);
	symbol->n_value = ft_swap_int64(symbol->n_value);
}

