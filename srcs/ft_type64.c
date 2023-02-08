/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 16:02:47 by zweng             #+#    #+#             */
/*   Updated: 2023/02/08 17:18:18 by vagrant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int	get_sym_type_suite_2(Elf64_Ehdr *ehdr, Elf64_Shdr *shdrt,
		Elf64_Sym cur_sym)
{
	unsigned int	c, bind;
    uint16_t        st_shndx;
    uint32_t        sh_type;
    uint64_t        sh_flags;

	bind = ELF64_ST_BIND(cur_sym.st_info);
    st_shndx = read_uint16(cur_sym.st_shndx);
    sh_type = read_uint32(shdrt[st_shndx].sh_type);
    sh_flags = read_uint64(shdrt[st_shndx].sh_flags);
    if (sh_type == SHT_PROGBITS &&
            sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'D';
    else if (sh_type == SHT_PROGBITS &&
            sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
        c = 'T';
    else if (sh_type == SHT_DYNAMIC)
        c = 'D';
	else
		c = ('t' - 32);
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

static unsigned int	get_sym_type_suite(Elf64_Ehdr *ehdr, Elf64_Shdr *shdrt,
		Elf64_Sym cur_sym)
{
	unsigned int	c, bind;
    uint16_t        st_shndx, e_shnum;
    uint32_t        sh_type;
    uint64_t        sh_flags;

	bind = ELF64_ST_BIND(cur_sym.st_info);
    st_shndx = read_uint16(cur_sym.st_shndx);
    e_shnum = read_uint16(ehdr->e_shnum);
	if (st_shndx == SHN_ABS)
		c = 'A';
	else if (st_shndx == SHN_COMMON)
		c = 'C';
	else if (st_shndx < e_shnum)
    {
        sh_type = read_uint32(shdrt[st_shndx].sh_type);
        sh_flags = read_uint64(shdrt[st_shndx].sh_flags);
	    if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
            c = 'B';
        else if ((sh_type == SHT_PROGBITS || sh_type == SHT_RELA ||
            sh_type == SHT_REL || sh_type == SHT_HASH ||
            sh_type == SHT_GNU_versym || sh_type == SHT_GNU_verdef ||
            sh_type == SHT_STRTAB || sh_type == SHT_DYNSYM ||
            sh_type == SHT_NOTE) && sh_flags == SHF_ALLOC)
        c = 'R';
        else
            return (get_sym_type_suite_2(ehdr, shdrt, cur_sym));
    }
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

unsigned int	get_sym_type64(Elf64_Ehdr *ehdr, Elf64_Shdr *shdrt,
		Elf64_Sym cur_sym)
{
	unsigned int	c, bind, type;
    uint16_t        st_shndx;

	bind = ELF64_ST_BIND(cur_sym.st_info);
	type = ELF64_ST_TYPE(cur_sym.st_info);
    st_shndx = read_uint16(cur_sym.st_shndx);
	if (bind == STB_GNU_UNIQUE)
		c = 'u';
	else if (bind == STB_WEAK)
	{
		c = 'W';
		if (st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (bind == STB_WEAK && type == STT_OBJECT)
	{
		c = 'V';
		if (st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (st_shndx == SHN_UNDEF)
		c = 'U';
	else
		return (get_sym_type_suite(ehdr, shdrt, cur_sym));
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}
