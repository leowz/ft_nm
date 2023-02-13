/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 17:25:25 by zweng             #+#    #+#             */
/*   Updated: 2023/02/13 15:00:41 by vagrant          ###   ########.fr       */
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

static void output_entry(t_symbol sym)
{
    unsigned int	shndx, value; 

    shndx = read_uint16(((Elf64_Sym *)sym.symptr)->st_shndx);
    value = read_uint64(((Elf64_Sym *)sym.symptr)->st_value);
    if (shndx != SHN_UNDEF)
        ft_printf("%016x %1c %s\n", value, sym.type, sym.name);
    else
        ft_printf("%s %1c %s\n", "                ", sym.type, sym.name);
}

static void print_symbols(t_array *arr, t_param params)
{
    uint32_t    i;
    t_symbol    *sym;
    t_arritem   *item;
    Elf64_Sym   *symptr;
    uint16_t    st_shndx;

    i = 0;
    if (!HAS_ARG(params, ARG_P))
    {
        ft_arrbubblesort(arr, itemcmp_value_asc);
        if (!HAS_ARG(params, ARG_R))
            ft_arrbubblesort(arr, itemcmp_asc);
        else 
            ft_arrbubblesort(arr, itemcmp_desc);
    }
    while (arr && i < arr->current_size)
    {
        item = ft_arritem_at(arr, i);
        sym = item->content;
        symptr = sym->symptr;
        st_shndx = read_uint16(symptr->st_shndx);
        if (HAS_ARG(params, ARG_U))
        {
            if (st_shndx == SHN_UNDEF) 
                output_entry(*sym);
        }
        else if (HAS_ARG(params, ARG_G))
        {
            if (ELF64_ST_BIND(symptr->st_info) == STB_GLOBAL ||
                    ELF64_ST_BIND(symptr->st_info) == STB_WEAK)
                output_entry(*sym);
        }
        else if (HAS_ARG(params, ARG_A))
            output_entry(*sym);
        else
            if ((!is_special_section_indice(st_shndx) &&
                        ELF64_ST_TYPE(symptr->st_info) != STT_SECTION))
                output_entry(*sym);
        i++;
    }
}

static void add_to_array(t_array **arr, unsigned char type, char *st_name,
        uint64_t st_value, uint16_t st_shndx, Elf64_Sym *symptr)
{
    t_symbol    *sym;

    if (!(*arr))
       *arr = ft_arrnew(); 
    if ((sym = malloc(sizeof(*sym))))
    {
        sym->symptr = symptr;
        sym->type = type;
        sym->name = st_name;
        sym->value = st_value;
        sym->index = st_shndx;
        ft_arrappend_raw(*arr, sym, sizeof(*sym));
    }
}

static int  handle_symtab(const void *file, size_t filesize, Elf64_Ehdr *ehdr,
        Elf64_Shdr *shdrt, unsigned int symtab_idx, t_param params)
{
    unsigned int    symtab_len, i, shstrtabidx;
    Elf64_Sym       *symtab;
    char            *strtab, *shstrtab, *name;
    unsigned char   type;
    t_array         *sym_arr;
    uint16_t        e_shstrndx, e_shnum, st_shndx;
    uint32_t        strtab_idx, symtab_st_name;
    uint64_t        strtab_sh_offset, strtab_sh_size, symtab_sh_offset,
                    symtab_sh_size, symtab_sh_entsize, shstr_sh_offset,
                    shstr_sh_size;

    sym_arr = NULL;
    strtab_idx = read_uint32(shdrt[symtab_idx].sh_link);
    e_shstrndx = read_uint16(ehdr->e_shstrndx);
    e_shnum = read_uint16(ehdr->e_shnum);
    strtab_sh_offset = read_uint64(shdrt[strtab_idx].sh_offset);
    strtab_sh_size = read_uint64(shdrt[strtab_idx].sh_size);
    symtab_sh_offset = read_uint64(shdrt[symtab_idx].sh_offset);
    symtab_sh_size = read_uint64(shdrt[symtab_idx].sh_size);
    symtab_sh_entsize = read_uint64(shdrt[symtab_idx].sh_entsize);
    shstr_sh_offset = read_uint64(shdrt[e_shstrndx].sh_offset);
    shstr_sh_size = read_uint64(shdrt[e_shstrndx].sh_size);
    if (sizeof(*symtab) !=  symtab_sh_entsize ||
        strtab_idx >= e_shnum || e_shstrndx >= e_shnum || 
        filesize < strtab_sh_offset + strtab_sh_size ||
        filesize < symtab_sh_offset + symtab_sh_size ||
        filesize < shstr_sh_offset + shstr_sh_size)
        return error_msg("Bad symtab\n");
    strtab = (char *)(file + strtab_sh_offset);
    shstrtab = (char *)(file + shstr_sh_offset);
    symtab = (Elf64_Sym *)(file + symtab_sh_offset);
    symtab_len = ft_symtab_counter(symtab_sh_size, sizeof(*symtab));
    i = 1;
    while (i < symtab_len)
    {
        symtab_st_name = read_uint32(symtab[i].st_name);
		name = strtab + symtab_st_name;
		type = get_sym_type64(ehdr, shdrt, symtab[i]);
        st_shndx = read_uint16(symtab[i].st_shndx);
        if (st_shndx < e_shnum)
        {
            shstrtabidx = read_uint32(shdrt[st_shndx].sh_name);
			if (ELF64_ST_TYPE(symtab[i].st_info) == STT_SECTION)
				name = shstrtab + shstrtabidx;
        }
		type = type_adjust(name, type, ELF64_ST_BIND(symtab[i].st_info));
        add_to_array(&sym_arr, type, name, read_uint64(symtab[i].st_value),
                st_shndx, symtab + i);
        i++;
    }
    print_symbols(sym_arr, params);
    delete_array(&sym_arr);
    return (FUN_SUCS);
}

int         ft_nm64(const void *file, size_t size, t_param params)
{
    Elf64_Ehdr      *ehdr;
    Elf64_Shdr      *shdrt;
    unsigned int    i;
    uint16_t        e_shstrndx, e_shnum;
    uint32_t        sh_type, sh_name;
    uint64_t        sh_size, e_shoff, sh_offset;

    if (size < sizeof(*ehdr))
       return error_msg("File size too small\n");
    ehdr = (Elf64_Ehdr *)file;
    e_shoff = read_uint64(ehdr->e_shoff);
    if (e_shoff > size)
        return error_msg("File e_shoff bigger than file size\n");
    shdrt = (Elf64_Shdr *)(file + e_shoff);
    sh_size = read_uint64(shdrt[0].sh_size);
    sh_offset = read_uint64(shdrt[0].sh_offset);
    e_shstrndx = read_uint16(ehdr->e_shstrndx);
    e_shnum = read_uint16(ehdr->e_shnum);
    sh_type = read_uint32(shdrt[e_shstrndx].sh_type);
    if (sh_size != 0 && sh_offset != 0)
        return error_msg("bad section table\n");
    if (e_shstrndx >= e_shnum || sh_type != SHT_STRTAB)
        return error_msg("bad section table header\n");
    i = 0;
    while (i < e_shnum)
    {
        sh_name = read_uint32(shdrt[i].sh_name);
        sh_size = read_uint64(shdrt[e_shstrndx].sh_size);
        sh_type = read_uint32(shdrt[i].sh_type);
        if (sh_name > sh_size)
            return error_msg("bad section table header\n");
        if (sh_type == SHT_SYMTAB)
            return handle_symtab(file, size, ehdr, shdrt, i, params);
        i++;
    }
    return error_msg("No Symtab found\n");
}
