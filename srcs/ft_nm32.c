/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2022/12/16 18:36:36 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned int ft_symtab_counter(unsigned int total, unsigned int size)
{
    unsigned int    counter, i;

    counter = 0;
    i = 0;
    while (i < total)
    {
       i += size; 
       counter++;
    }
   return counter; 
}

static int  output_entry(t_symbol sym)
{
    unsigned int    value; 

    value = ((Elf32_Sym *)sym.symptr)->st_value;
    if (!value)
        ft_printf("%s %1c %s\n", "        ", sym.type, sym.name);
    else
        ft_printf("%08x %1c %s\n", value, sym.type, sym.name);
}

static void print_symbols(t_array *arr, t_param params)
{
    int         i;
    t_symbol    *sym;
    t_arritem   *item;
    Elf32_Sym   *symptr;

    i = 0;
    if (!HAS_ARG(params, ARG_P))
        if (!HAS_ARG(params, ARG_R))
            ft_arrbubblesort(arr, itemcmp_asc);
        else 
            ft_arrbubblesort(arr, itemcmp_desc);
    while (arr && i < arr->current_size)
    {
        item = ft_arritem_at(arr, i);
        sym = item->content;
        symptr = sym->symptr;
        if (!HAS_ARG(params, ARG_A))
        {
            if ((!is_special_section_indice(symptr->st_shndx) &&
                        ELF32_ST_TYPE(symptr->st_info) != STT_SECTION))
                output_entry(*sym);
        }
        else
           output_entry(*sym);
        i++;
    }
}

static void add_to_array(t_array **arr, unsigned char type,
        char *name, Elf32_Sym *symptr)
{
    t_symbol    *sym;

    if (!(*arr))
       *arr = ft_arrnew(); 
    if (sym = malloc(sizeof(*sym)))
    {
        sym->symptr = symptr;
        sym->type = type;
        sym->name = name;
        ft_arrappend_raw(*arr, sym, sizeof(*sym));
    }
}

static int  handle_symtab(const void *file, size_t filesize, Elf32_Ehdr *ehdr,
        Elf32_Shdr *shdrt, unsigned int symtab_idx, t_param params)
{
    unsigned int    symtab_len, strtab_idx, i, name_idx, shstrtabidx;
    Elf32_Sym       *symtab;
    char            *strtab, *shstrtab, *name;
    unsigned char   type;
    t_array         *sym_arr;

    sym_arr = NULL;
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
    symtab = (Elf32_Sym *)(file + shdrt[symtab_idx].sh_offset);
    symtab_len = ft_symtab_counter(shdrt[symtab_idx].sh_size,
            sizeof(*symtab));
    i = 1;
    while (i < symtab_len)
    {
        name_idx = symtab[i].st_name;
        if (symtab[i].st_shndx < ehdr->e_shnum)
        {
            shstrtabidx = shdrt[symtab[i].st_shndx].sh_name;
            type = get_sym_type((shstrtab + shstrtabidx),
                    ELF32_ST_BIND(symtab[i].st_info),
                    ELF32_ST_TYPE(symtab[i].st_info), symtab[i].st_value);
            name = strtab + name_idx;
            add_to_array(&sym_arr, type, name, symtab + i);
        }
        i++;
    }
    print_symbols(sym_arr, params);
    delete_array(&sym_arr);
    return (FUN_SUCS);
}

int         ft_nm32(const void *file, size_t size, t_param params)
{
    // ELF Header
    Elf32_Ehdr      *ehdr;
    // ELF Section Header table
    Elf32_Shdr      *shdrt;
    unsigned int    i;

    //ft_printf("file addr: %p\n", file);
    if (size < sizeof(*ehdr))
       return error_msg("File size too small\n");
    ehdr = (Elf32_Ehdr *)file;
    shdrt = (Elf32_Shdr *)(file + ehdr->e_shoff);
    // check initial entry of section table, all fields to zero
    if (shdrt[0].sh_size != 0 && shdrt[0].sh_offset != 0)
        return error_msg("bad section table\n");
    // check section header name string table
    if (ehdr->e_shstrndx >= ehdr->e_shnum ||
            shdrt[ehdr->e_shstrndx].sh_type != SHT_STRTAB)
        return error_msg("bad section table header\n");
    i = 0;
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
