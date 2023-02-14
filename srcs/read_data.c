/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 17:40:02 by zweng             #+#    #+#             */
/*   Updated: 2023/02/14 13:32:40 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	reverse_endian(void *ptr, size_t length)
{
	uint8_t *bytes;
	uint8_t tmp[32];
    size_t  i;

    bytes = (uint8_t *)ptr;
    i = 0;
    while (i < length)
    {
		tmp[i] = bytes[length - 1 - i];
        i++;
    }
    i = 0;
    while (i < length)
    {
		bytes[i] = tmp[i];
        i++;
    }
}

uint16_t	read_uint16(uint16_t nb)
{
	uint16_t	res;
    int         endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(uint16_t));
	return (res);
}

uint32_t	read_uint32(uint32_t nb)
{
	uint32_t	res;
    int         endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(uint32_t));
	return (res);
}

uint64_t	read_uint64(uint64_t nb)
{
	uint64_t	res;
    int         endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(uint64_t));
	return (res);
}

unsigned int	read_unsigned_int(unsigned int nb)
{
	unsigned int	res;
    int         endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(unsigned int));
	return (res);
}

long unsigned int	read_long_unsigned_int(long unsigned int nb)
{
	long unsigned int	res;
    int         endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(long unsigned int));
	return (res);
}

int			read_int(int nb)
{
	int     res;
    int     endian;

    endian = need_change_endian(NULL);
	res = nb;
	if (endian)
		reverse_endian(&res, sizeof(int));
	return (res);
}
