/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 16:02:47 by zweng             #+#    #+#             */
/*   Updated: 2023/02/07 18:40:08 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int	get_sym_type_suite_2(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
		Elf32_Sym cur_sym)
{
	unsigned int	c, bind;
	Elf32_Shdr		cur_shdr;
    uint16_t        st_shndx;

	bind = ELF32_ST_BIND(cur_sym.st_info);
    st_shndx = read_uint16(cur_sym.st_shndx);
	cur_shdr = shdrt[st_shndx];
    if (shdrt[st_shndx].sh_type == SHT_PROGBITS &&
            shdrt[st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'D';
    else if (shdrt[st_shndx].sh_type == SHT_PROGBITS &&
            shdrt[st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
        c = 'T';
    else if (shdrt[st_shndx].sh_type == SHT_DYNAMIC)
        c = 'D';
	else
		c = ('t' - 32);
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

static unsigned int	get_sym_type_suite(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
		Elf32_Sym cur_sym)
{
	unsigned int	c, bind;
	Elf32_Shdr		cur_shdr;
    uint16_t        st_shndx, e_shnum;

	bind = ELF32_ST_BIND(cur_sym.st_info);
    st_shndx = read_uint16(cur_sym.st_shndx);
    e_shnum = read_uint16(ehdr->e_shnum);
	if (cur_sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (cur_sym.st_shndx == SHN_COMMON)
		c = 'C';
    else if (st_shndx< e_shnum)
    {
        if (shdrt[st_shndx].sh_type == SHT_NOBITS &&
                shdrt[st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
            c = 'B';
        else if ((shdrt[st_shndx].sh_type == SHT_PROGBITS ||
                shdrt[st_shndx].sh_type == SHT_RELA ||
                shdrt[st_shndx].sh_type == SHT_REL ||
                shdrt[st_shndx].sh_type == SHT_HASH ||
                shdrt[st_shndx].sh_type == SHT_GNU_versym ||
                shdrt[st_shndx].sh_type == SHT_GNU_verdef ||
                shdrt[st_shndx].sh_type == SHT_STRTAB ||
                shdrt[st_shndx].sh_type == SHT_DYNSYM ||
                shdrt[st_shndx].sh_type == SHT_NOTE)
                && shdrt[st_shndx].sh_flags == SHF_ALLOC)
            c = 'R';
        else
            return (get_sym_type_suite_2(ehdr, shdrt, cur_sym));
    }
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

unsigned int	get_sym_type32(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
		Elf32_Sym cur_sym)
{
	unsigned int	c, bind, type;

	bind = ELF32_ST_BIND(cur_sym.st_info);
	type = ELF32_ST_TYPE(cur_sym.st_info);
	if (bind == STB_GNU_UNIQUE)
		c = 'u';
	else if (bind == STB_WEAK)
	{
		c = 'W';
		if (cur_sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (bind == STB_WEAK && type == STT_OBJECT)
	{
		c = 'V';
		if (cur_sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (cur_sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else
		return (get_sym_type_suite(ehdr, shdrt, cur_sym));
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}
