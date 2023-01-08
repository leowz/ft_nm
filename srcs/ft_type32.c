/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 16:02:47 by zweng             #+#    #+#             */
/*   Updated: 2023/01/08 17:56:29 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int	get_sym_type_suite_2(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
		Elf32_Sym cur_sym)
{
	unsigned int	c, bind, type;
	Elf32_Shdr		cur_shdr;

	bind = ELF32_ST_BIND(cur_sym.st_info);
	type = ELF32_ST_TYPE(cur_sym.st_info);
	if (cur_sym.st_shndx < ehdr->e_shnum)
	{
		cur_shdr = shdrt[cur_sym.st_shndx];
		if (cur_shdr.sh_type == SHT_PROGBITS &&
				cur_shdr.sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'D';
		else if (cur_shdr.sh_type == SHT_PROGBITS &&
				cur_shdr.sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
			c = 'T';
		else if (cur_shdr.sh_type == SHT_DYNAMIC)
			c = 'D';
	}
	else
		c = ('t' - 32);
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

static unsigned int	get_sym_type_suite(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
		Elf32_Sym cur_sym)
{
	unsigned int	c, bind, type;
	Elf32_Shdr		cur_shdr;

	bind = ELF32_ST_BIND(cur_sym.st_info);
	type = ELF32_ST_TYPE(cur_sym.st_info);
	if (cur_sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (cur_sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (cur_sym.st_shndx < ehdr->e_shnum)
	{
		cur_shdr = shdrt[cur_sym.st_shndx];
		if (cur_shdr.sh_type == SHT_NOBITS &&
				cur_shdr.sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'B';
		else if (cur_shdr.sh_type == SHT_PROGBITS &&
				cur_shdr.sh_flags == SHF_ALLOC)
			c = 'R';
	}
	else
		return (get_sym_type_suite_2(ehdr, shdrt, cur_sym));
	if (bind == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

unsigned int	get_sym_type2(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
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
