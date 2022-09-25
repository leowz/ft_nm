#include "ft_nm.h"

static int  get_header_convert(const t_arch input, const void *add,
                                                struct mach_header_64 *header)
{
    if (secure_add(input, add, sizeof(struct mach_header)))
    {
        *header = *(struct mach_header_64 *)add;
        if (input.is_swap)
            swap_mach_header_64(header);
        return (0);
    }
    return (1);
}

static t_list       *create_sym_64(t_arch input, char *string_table,
                                                struct nlist_64 sym_table)
{
    t_sym       *elem_sym;
    t_list      *elem;

    if (input.is_swap)
        swap_nlist_64(&sym_table);
    if (!(elem_sym = (t_sym *)malloc(sizeof(t_sym))))
    {
        (void)print_error(ERR_MALLOC, 0);
        return (NULL);
    }
    elem_sym->name = secure_string(input, string_table, sym_table.n_un.n_strx);
    elem_sym->value = sym_table.n_value;
    elem_sym->arch = ARCH_64;
    elem_sym->ntype = sym_table.n_type;
    if (find_seg_sect_name_64(sym_table, elem_sym, input))
    {
       free(elem_sym);
       return (NULL);
    }
    elem = ft_lstnew(elem_sym, sizeof(t_sym));
    return (elem);
}

static int      loop_get_list_sym_64(t_arch *input, struct symtab_command sym,
                                        struct nlist_64 *array, char *string)
{
    int64_t     index;
    t_list      *new;
    t_list      *lst;

    lst = NULL;
    index = 0;
    while(index < sym.nsyms)
    {
        if (!(new = create_sym_64(*input, string, array[index])))
           return (1);
        ft_lstappend(&lst, new);
        index++;
    }
    input->list = lst;
    return (0);
}

static t_list       *get_list_syms_64(struct symtab_command sym, t_arch input)
{
    char                *string;
    struct nlist_64     *array;

    if (input.is_swap)
        swap_symtab_command(&sym);
    if (!(array = secure_add_mv(input, input.data, sym.symoff)))
    {
        print_error(ERR_MALFORMED, 0);
        return (NULL);
    }
    if (!(string = secure_add_mv(input, input.data, sym.stroff)))
    {
        print_error(ERR_MALFORMED, 0);
        return (NULL);
    }
    if (loop_get_list_sym_64(&input, sym, array, string))
    {
        print_error(ERR_MALFORMED, 0);
        return (NULL);
    }
    return (input.list);
}

int     handler_64(t_arch *arch)
{
    struct mach_header_64        header;
    struct symtab_command      *sym;

    if (get_header_convert(*arch, arch->data, &header))
        return (print_error("Invalid Format", 2));
    arch->ncmds = header.ncmds;
    arch->cpu_type = header.cputype;
    arch->cpu_subtype = header.cpusubtype;
    if (!(arch->lc = secure_add_mv(*arch, arch->data,
                                            sizeof(struct mach_header_64))))
        return (print_error("Invalid", 2));
    if (!(sym = get_symtab_lc(*arch)))
        return (1);
    arch->list = get_list_syms_64(*sym, *arch);
    if (!arch->list)
        return (1);
    sort_sym(&(arch->list));
    print_nm(arch);
    return (0);
}

