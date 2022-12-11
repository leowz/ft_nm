/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2022/12/11 19:34:28 by zweng            ###   ########.fr       */
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

static int  output_entry(t_symbol sym)
{
    if (sym.value)
        ft_printf("%s %c %s\n", "                ", sym.type, sym.name);
    else
        ft_printf("%016x %c %s\n", sym.value, sym.type, sym.name);
}

static void print_symbols(t_array *arr, t_param params)
{
    (void)params;
    int     i;

    i = 0;
    while (arr && i < len)
    {
        output_entry(arr[i++]);
    }
}

static int  handle_symtab(const void *file, size_t filesize, Elf64_Ehdr *ehdr,
        Elf64_Shdr *shdrt, unsigned int symtab_idx, t_param params)
{
    unsigned int    symtab_len, strtab_idx, i, name_idx, shstrtabidx, value;
    Elf64_Sym       *symtab;
    char            *strtab, *shstrtab, *name;
    unsigned char   type;
    t_array         *sym_arr;

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
    strtab = (char *)(file + shdrt[strtab_idx].sh_offset);
    shstrtab = (char *)(file + shdrt[ehdr->e_shstrndx].sh_offset);
    symtab = (Elf64_Sym *)(file + shdrt[symtab_idx].sh_offset);
    symtab_len = ft_symtab_counter(shdrt[symtab_idx].sh_size,
            sizeof(*symtab));
    /*if (!(sym_arr = malloc(sizeof(t_symbol) * (symtab_len + 1))))
        return error_msg("sym_arr not allocated\n");
    ft_bzero(sym_arr, sizeof(t_symbol) * (symtab_len + 1));*/
    i = 0;
    while (i < symtab_len)
    {
        name_idx = (symtab + i)->st_name;
        if (symtab[i].st_shndx < ehdr->e_shnum)
        {
            shstrtabidx = shdrt[symtab[i].st_shndx].sh_name;
            type = get_sym_type((shstrtab + shstrtabidx),
                    ELF64_ST_BIND((symtab + i)->st_info),
                    ELF64_ST_TYPE((symtab + i)->st_info),
                    (symtab + i)->st_value);
            value = symtab[i].st_value;
            name = strtab + name_idx;
            add_to_array(&sym_arr, value, type, name);
        }
        else
        {
           //printf("no strtabidx\n");
        }
        /*if ((!is_special_section_indice(symtab[i].st_shndx) &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_NOTYPE &&
               ELF64_ST_TYPE(symtab[i].st_info) != STT_SECTION))
        {  
    1   (void)params;
           counter++;
        }*/
        i++;
    }
    print_symbols(sym_arr, params);
    //free(sym_arr);
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
