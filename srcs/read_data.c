/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 17:40:02 by zweng             #+#    #+#             */
/*   Updated: 2023/01/09 17:41:02 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	reverse_endian(void *ptr, size_t length)
{
	uint8_t *bytes = (uint8_t *)ptr;
	uint8_t tmp[32] = { 0 };

	for (size_t i = 0; i < length; i++)
	{
		tmp[i] = bytes[length - 1 - i];
	}
	for (size_t i = 0; i < length; i++)
	{
		bytes[i] = tmp[i];
	}
}

uint16_t	read_uint16(uint16_t nb, int endian)
{
	uint16_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint16_t));
	return (res);
}

uint32_t	read_uint32(uint32_t nb, int endian)
{
	uint32_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint32_t));
	return (res);
}

uint64_t	read_uint64(uint64_t nb, int endian)
{
	uint64_t	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(uint64_t));
	return (res);
}

unsigned int	read_unsigned_int(unsigned int nb, int endian)
{
	unsigned int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(unsigned int));
	return (res);
}

long unsigned int	read_long_unsigned_int(long unsigned int nb, int endian)
{
	long unsigned int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(long unsigned int));
	return (res);
}

int			read_int(int nb, int endian)
{
	int	res;

	res = nb;
	if (endian & OPT_BIG_ENDIAN)
		reverse_endian(&res, sizeof(int));
	return (res);
}
