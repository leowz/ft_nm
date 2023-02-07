/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2023/02/07 16:42:15 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int check_elf_ident(char *file, unsigned char *arch, size_t size)
{
    if (size < 16)
        return (FUN_FAIL);
    *arch = file[EI_CLASS];
    is_big_endian(file);
    if (ft_memcmp(file, ELFMAG, SELFMAG) != 0 || 
            file[EI_CLASS] <= 0 || file[EI_CLASS] > 2 ||
            file[EI_DATA] <= 0 || file[EI_DATA] > 2 ||
            file[EI_VERSION] != EV_CURRENT)
        return (0);
    else
        return (1);
}

/*
 * big endian return 1, small endian return 0
 * else return -1
 *
*/

int is_big_endian(char *file)
{
    static unsigned char endian;
    static int  init = 0;

    if (!init)
    {
        endian = file[EI_DATA];
        init = 1;
    }
    if (endian == ELFDATA2LSB)
        return (0);
    else if (endian == ELFDATA2MSB)
        return (1);
    else
        return (-1);
}

int is_special_section_indice(uint16_t s_idx)
{
    return (s_idx == SHN_LOPROC || s_idx == SHN_BEFORE || s_idx == SHN_AFTER ||
            s_idx == SHN_HIPROC || s_idx == SHN_LOOS || s_idx == SHN_HIOS ||
            s_idx == SHN_ABS || s_idx == SHN_COMMON || s_idx == SHN_XINDEX ||
            s_idx == SHN_HIRESERVE);
}

unsigned char	get_ssi_type(unsigned int s_idx)
{
	if (s_idx == SHN_UNDEF)
		return ('u');
	if (s_idx == SHN_COMMON)
		return ('c');
	else
		return ('a');
}

unsigned char   type_adjust(const char *sname, char type, unsigned int bind)
{
    char    c;

    c = 0;
    if (!ft_strncmp(sname, ".comment", ft_strlen(".comment")) || 
            !ft_strncmp(sname, ".copyright", ft_strlen(".copyright")) ||
            !ft_strncmp(sname, ".gnu_debug", ft_strlen(".gnu_debug")) ||
            !ft_strncmp(sname, ".ident", ft_strlen(".ident")) ||
            !ft_strncmp(sname, ".SUNW", ft_strlen(".SUNW")))
        c = 'n';
    if (!ft_strncmp(sname, ".debug_", ft_strlen(".debug_")))
        c = 'N';
    if (!ft_strncmp(sname, ".gnu.hash", ft_strlen(".gnu.hash")) ||
            !ft_strncmp(sname, ".rodata", ft_strlen(".rodata")) ||
            !ft_strncmp(sname, ".rela.plt", ft_strlen(".rela.plt")) ||
            !ft_strncmp(sname, ".gnu.version_r", ft_strlen(".gnu.version_r")))
    {
        c = 'R';
        if (bind == STB_LOCAL)
            c += 32;
    }
    if (c)
        return (c);
    return (type);
}
