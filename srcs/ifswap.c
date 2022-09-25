#include "ft_nm.h"

uint16_t	ifswap16(t_arch *input, uint16_t x)
{
	if (input->is_swap)
		return (ft_swap_int16(x));
	return (x);
}

uint32_t	ifswap32(t_arch *input, uint32_t x)
{
	if (input->is_swap)
		return (ft_swap_int32(x));
	return (x);
}

uint64_t	ifswap64(t_arch *input, uint64_t x)
{
	if (input->is_swap)
		return (ft_swap_int64(x));
	return (x);
}
