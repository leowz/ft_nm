/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2022/11/25 18:04:31 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int ft_symtab_counter(unsigned int symtab_size,
        unsigned int entry_size)
{
    unsigned int    counter;
    int             i;

    i = 0;
    counter = 0;
    while (i < symtab_size)
    {
       counter++;
       i += entry_size;
    }
    return counter;
}

static int  output_entry()
{
    i = 0;
    while (i < symtab_count)
    {
        if (is_special_section_indice(symtab[i].st_shndex) ||
                symtab[i].info == STT_NOTYPE || 
                ELF32_ST_TYPE(symtab[i].st_info) == STT_SECTION)
            continue ;
        print_entry()
        i++
    }

}

static int  handle_symtab(const void *file, ELF32_Ehdr *ehdr,
        ELF32_Shdr **shdrt, unsigned int symtab_idx, t_param params)
{
    unsigned int    symtab_len, strtab;
    ELF32_Sym       *symtab;
    char            **strtab;
    char            **shstrtab;

    strtab = shdrt[symtab].sh_link;
    /* checks */
    if (sizeof(*symtab) != shdrt[symtab_idx]->sh_entsize ||
        shdrt[symtab_idx]->sh_link >= ehdr->e_shnum ||
        ehdr->e_shstrndx >= ehdr->e_shnum || 
        filesize < shdrt[strtab]->sh_offset + shdrt[strtab]->sh_size ||
        filesize < shdrt[symtab_idx]->sh_offset + shdrt[symtab_idx]->sh_size ||
        filesize < shdrt[ehdr->e_shstrndx]->sh_offset +
        shdrt[ehdr->e_shstrndx]->sh_size)
        return error_msg("Bad symtab\n");
    /* get addresses */
    strtab = (char **)(file + shdrt[strtab]->sh_offset);
    shstrtab = (char **)(file + shdrt[ehdr->e_shstrndex]->sh_offset);
    symtab = (ELF32_Sym *)(file + shdrt[symtab_idx].sh_offset);
    symtab_count = ft_symtab_counter(shdrt[symtab_idx]->sh_size,
            sizeof(*symtab));
    return output_entry();
}

int         ft_nm32(const void *file, size_t size, t_param params)
{
    // ELF Header
    ELF32_Ehdr      *ehdr;
    // ELF Section Header table
    ELF32_Shdr      **shdrt;
    unsigned int    i;
    int             ret;

    if (size < sizeof(*ehdr))
       return error_msg("File size too small\n");
    ehdr = (ELF32_Ehdr *)file;
    if (!check_elf(*ehdr, size))
        return error_msg("elf header bad format\n");
    shdrt = (ELF32_Shdr **)(file + ehdr->e_shoff);
    // check initial entry of section table, all fields to zero
    if (shdrt[0].sh_size != 0 && shdrt[0].shoffset != 0)
        return error_msg("bad section table\n");
    // check section header name string table
    if (ehdr->e_shstrndex >= ehdr.e_shnum ||
            shdrt[ehdr->e_shstrndex].sh_type != SHT_STRTAB)
        return error_msg("bad section table header\n");
    i = 0;
    while (i < ehdr->e_shnum)
    {
       // some checks 
       if (shdrt[i]->sh_name > shdrt[ehdr->e_shstrndex]->sh_size)
           return error_msg("bad section table header\n");
       if (shdrt[i]->sh_type == SHT_SYMTAB)
           return handle_symtab(file, ehdr, shdrt, i, params);
       i++;
    }
    return error_msg("No Symtab found\n");
}
