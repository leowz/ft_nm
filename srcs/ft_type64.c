/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 16:02:47 by zweng             #+#    #+#             */
/*   Updated: 2023/01/09 15:56:47 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int	get_sym_type_suite_2(Elf64_Ehdr *ehdr, Elf64_Shdr *shdrt,
		Elf64_Sym cur_sym)
{
	unsigned int	c, bind, type;
	Elf64_Shdr		cur_shdr;

	bind = ELF64_ST_BIND(cur_sym.st_info);
	type = ELF64_ST_TYPE(cur_sym.st_info);
	cur_shdr = shdrt[cur_sym.st_shndx];
    if (shdrt[cur_sym.st_shndx].sh_type == SHT_PROGBITS &&
            shdrt[cur_sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'D';
    else if (shdrt[cur_sym.st_shndx].sh_type == SHT_PROGBITS &&
            shdrt[cur_sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
        c = 'T';
    else if (shdrt[cur_sym.st_shndx].sh_type == SHT_DYNAMIC)
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
	unsigned int	c, bind, type;
	Elf64_Shdr		cur_shdr;

	bind = ELF64_ST_BIND(cur_sym.st_info);
	type = ELF64_ST_TYPE(cur_sym.st_info);
	if (cur_sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (cur_sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (cur_sym.st_shndx < ehdr->e_shnum)
    {
	    if (shdrt[cur_sym.st_shndx].sh_type == SHT_NOBITS &&
            shdrt[cur_sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
            c = 'B';
        else if ((shdrt[cur_sym.st_shndx].sh_type == SHT_PROGBITS ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_RELA ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_REL ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_HASH ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_GNU_versym ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_GNU_verdef ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_STRTAB ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_DYNSYM ||
            shdrt[cur_sym.st_shndx].sh_type == SHT_NOTE)
            && shdrt[cur_sym.st_shndx].sh_flags == SHF_ALLOC)
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

	bind = ELF64_ST_BIND(cur_sym.st_info);
	type = ELF64_ST_TYPE(cur_sym.st_info);
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
