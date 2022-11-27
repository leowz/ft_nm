/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2022/11/27 19:26:27 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int ft_symtab_counter(unsigned int symtab_size,
        unsigned int entry_size)
{
    unsigned int    counter, i;

    i = 0;
    counter = 0;
    while (i < symtab_size)
    {
       counter++;
       i += entry_size;
    }
    return counter;
}

/*static int  output_entry()
{
    i = 0;
    while (i < symtab_count)
    {
        if (is_special_section_indice(symtab[i].st_shndex) ||
                symtab[i].info == STT_NOTYPE || 
                ELF64_ST_TYPE(symtab[i].st_info) == STT_SECTION)
            continue ;
        print_entry()
        i++
    }

}*/

static int  handle_symtab(const void *file, size_t filesize, Elf64_Ehdr *ehdr,
        Elf64_Shdr *shdrt, unsigned int symtab_idx, t_param params)
{
    unsigned int    symtab_len, strtab_idx, i, name_idx, shstrtabidx;
    Elf64_Sym       *symtab;
    char            **strtab;
    char            **shstrtab;

    ft_putstr("set strtab_idx\n");
    strtab_idx = shdrt[symtab_idx].sh_link;
    /* checks */
    if (sizeof(*symtab) != shdrt[symtab_idx].sh_entsize ||
        shdrt[symtab_idx].sh_link >= ehdr->e_shnum ||
        ehdr->e_shstrndx >= ehdr->e_shnum || 
        filesize < shdrt[strtab_idx].sh_offset + shdrt[strtab_idx].sh_size ||
        filesize < shdrt[symtab_idx].sh_offset + shdrt[symtab_idx].sh_size ||
        filesize < shdrt[ehdr->e_shstrndx].sh_offset +
        shdrt[ehdr->e_shstrndx].sh_size)
        return error_msg("Bad symtab\n");
    /* get addresses */
    ft_putstr("before get addresses\n");
    strtab = (char **)(file + shdrt[strtab_idx].sh_offset);
    shstrtab = (char **)(file + shdrt[ehdr->e_shstrndx].sh_offset);
    (void)shstrtab;
    symtab = (Elf64_Sym *)(file + shdrt[symtab_idx].sh_offset);
    symtab_len = ft_symtab_counter(shdrt[symtab_idx].sh_size,
            sizeof(*symtab));
    i = 1;
    ft_putstr("before loop for print\n");
    while (i < symtab_len)
    {
        shstrtabidx = shdrt[symtab[i].st_shndx].sh_name;
        printf("section table name: %s\n", shstrtab[shstrtabidx]);
        name_idx = symtab[i].st_name;
        printf("name_idx %d\n", name_idx);
        printf("%s\n", strtab[name_idx]);
        if ((!is_special_section_indice(symtab[i].st_shndx) &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_NOTYPE &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_SECTION))
        {  
           (void)params;
        }
        i++;
    }
    return (FUN_SUCS);
}

int         ft_nm64(const void *file, size_t size, t_param params)
{
    // ELF Header
    Elf64_Ehdr      *ehdr;
    // ELF Section Header table
    Elf64_Shdr      *shdrt;
    unsigned int    i;
    //int             ret;

    ft_putstr("enter 64\n");
    if (size < sizeof(*ehdr))
       return error_msg("File size too small\n");
    ft_putstr("before 64 ehdr\n");
    ehdr = (Elf64_Ehdr *)file;
//    if (!check_elf(*ehdr, size))
 //       return error_msg("elf header bad format\n");
    ft_putstr("before shdrt\n");
    shdrt = (Elf64_Shdr *)(file + ehdr->e_shoff);
    // check initial entry of section table, all fields to zero
    ft_putstr("before size check\n");
    if (shdrt[0].sh_size != 0 && shdrt[0].sh_offset != 0)
        return error_msg("bad section table\n");
    // check section header name string table
    ft_putstr("before shstrndx check\n");
    if (ehdr->e_shstrndx >= ehdr->e_shnum ||
            shdrt[ehdr->e_shstrndx].sh_type != SHT_STRTAB)
        return error_msg("bad section table header\n");
    i = 0;
    ft_putstr("before loop\n");
    while (i < ehdr->e_shnum)
    {
       // some checks 
        ft_putstr("before check loop\n");
       if (shdrt[i].sh_name > shdrt[ehdr->e_shstrndx].sh_size)
           return error_msg("bad section table header\n");
        ft_putstr("before handle loop\n");
       if (shdrt[i].sh_type == SHT_SYMTAB)
           return handle_symtab(file, size, ehdr, shdrt, i, params);
       i++;
    }
    return error_msg("No Symtab found\n");
}
