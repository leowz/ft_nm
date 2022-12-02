/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2022/12/01 18:31:31 by zweng            ###   ########.fr       */
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
    char            *strtab;
    char            *shstrtab;

    //ft_putstr("set strtab_idx\n");
    strtab_idx = shdrt[symtab_idx].sh_link;
    //ft_printf("strtab index %d\n", strtab_idx);
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
    strtab = (char *)(file + shdrt[strtab_idx].sh_offset);

    shstrtab = (char *)(file + shdrt[ehdr->e_shstrndx].sh_offset);
    //ft_printf("symtab offset %x\n", shdrt[symtab_idx].sh_offset);
    symtab = (Elf64_Sym *)(file + shdrt[symtab_idx].sh_offset);
    symtab_len = ft_symtab_counter(shdrt[symtab_idx].sh_size,
            sizeof(*symtab));
    i = 0;
    //ft_printf("symbols\n");
    int counter = 0;
    while (i < symtab_len)
    {
        unsigned char t;
        (void)t;
        name_idx = (symtab + i)->st_name;
        //ft_printf("i %d, shstrtabidx %d\n",i, shstrtabidx);
        //ft_printf("symtab[%d].st_shndx: %d\n", i, symtab[i].st_shndx);
        if (symtab[i].st_shndx < ehdr->e_shnum) {
            shstrtabidx = shdrt[symtab[i].st_shndx].sh_name;
            //ft_printf("shstr %s\n",(shstrtab + shstrtabidx));
            t = get_sym_type((shstrtab + shstrtabidx),
                    ELF64_ST_BIND((symtab + i)->st_info),
                    ELF64_ST_TYPE((symtab + i)->st_info),
                    (symtab + i)->st_value);
            ft_printf("%016x %c %s\n", (symtab + i)->st_value,
                      t, (strtab + name_idx));
        }
        else
        {
            //ft_printf("No shstrtabidx\n");
        }
        (void)name_idx;
        (void)strtab;
        /*if ((!is_special_section_indice(symtab[i].st_shndx) &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_NOTYPE &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_SECTION))
        {  
           (void)params;
           counter++;
        }*/
        (void)params;
        i++;
    }
    //ft_printf("symbols %d\n", counter);
    return (FUN_SUCS);
}

int         ft_nm64(const void *file, size_t size, t_param params)
{
    // ELF Header
    Elf64_Ehdr      *ehdr;
    // ELF Section Header table
    Elf64_Shdr      *shdrt;
    unsigned int    i, symtab_idx, strtab_idx;
    //int             ret;

    //ft_printf("file addr: %p\n", file);
    if (size < sizeof(*ehdr))
       return error_msg("File size too small\n");
    ehdr = (Elf64_Ehdr *)file;
//    if (!check_elf(*ehdr, size))
 //       return error_msg("elf header bad format\n");
    shdrt = (Elf64_Shdr *)(file + ehdr->e_shoff);
    // check initial entry of section table, all fields to zero
    //ft_putstr("before size check\n");
    if (shdrt[0].sh_size != 0 && shdrt[0].sh_offset != 0)
        return error_msg("bad section table\n");
    // check section header name string table
    //ft_putstr("before shstrndx check\n");
    if (ehdr->e_shstrndx >= ehdr->e_shnum ||
            shdrt[ehdr->e_shstrndx].sh_type != SHT_STRTAB)
        return error_msg("bad section table header\n");
    i = 0;
    (void)symtab_idx;
    (void)strtab_idx;
    //ft_printf("ehdr num: %d\n", ehdr->e_shnum);
    while (i < ehdr->e_shnum)
    {
       if (shdrt[i].sh_name > shdrt[ehdr->e_shstrndx].sh_size)
           return error_msg("bad section table header\n");
       if (shdrt[i].sh_type == SHT_SYMTAB)
           return handle_symtab(file, size, ehdr, shdrt, i, params);
       i++;
    }
    return error_msg("No Symtab found\n");
}
