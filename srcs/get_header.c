#include "ft_nm.h"

int			get_header_fat(const t_arch input, void const *add,
													struct fat_header *header)
{
	if (secure_add(input, add, sizeof(struct mach_header_64)))
	{
		*header = *(struct fat_header *)add;
		if (input.is_swap)
			swap_fat_header(header);
		return (0);
	}
	return (1);
}

int			get_header_lib(const t_arch input, void const *add,
														struct ar_hdr *header)
{
	if (secure_add(input, add, sizeof(struct ar_hdr) + SARMAG))
	{
			*header = *(struct ar_hdr *)((void *)add + SARMAG);
			return (0);
		}
		return (1);
	}


