/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2023/02/14 12:20:41 by vagrant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int check_elf_ident(char *file, unsigned char *arch, size_t size)
{
    if (size < 16)
        return (FUN_FAIL);
    *arch = file[EI_CLASS];
    need_change_endian(file);
    if (ft_memcmp(file, ELFMAG, SELFMAG) != 0 || 
            file[EI_CLASS] <= 0 || file[EI_CLASS] > 2 ||
            file[EI_DATA] <= 0 || file[EI_DATA] > 2 ||
            file[EI_VERSION] != EV_CURRENT)
        return (0);
    else
        return (1);
}

static int get_machine_endian()
{
    int i;
    
    i = 1;
    if (*((char *)&i) == 1)
        return (ELFDATA2LSB);
    else
        return (ELFDATA2MSB);
}

/*
 * big endian return 1, small endian return 0
 * else return -1
 *
*/

int need_change_endian(char *file)
{
    static unsigned char file_endian;
    static unsigned char mach_endian;
    static int  init = 0;

    if (!init)
    {
        file_endian = file[EI_DATA];
        mach_endian = get_machine_endian();
        init = 1;
    }
    if (file_endian == mach_endian)
        return (0);
    else
        return (1);
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
    if (!ft_strncmp(sname, ".symtab", ft_strlen(".symtab")) || 
            !ft_strncmp(sname, ".strtab", ft_strlen(".strtab")) ||
            !ft_strncmp(sname, ".shstrtab", ft_strlen(".shstrtab")))
        c = 'a';
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
            !ft_strncmp(sname, ".dynstr", ft_strlen(".dynstr")) ||
            !ft_strncmp(sname, ".rela.plt", ft_strlen(".rela.plt")) ||
            !ft_strncmp(sname, ".rel.plt", ft_strlen(".rel.plt")) ||
            !ft_strncmp(sname, ".SUNW_version", ft_strlen(".SUNW_version")) ||
            !ft_strncmp(sname, ".rela.bss", ft_strlen(".rela.bss")) ||
            !ft_strncmp(sname, ".rela.got", ft_strlen(".rela.got")) ||
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
