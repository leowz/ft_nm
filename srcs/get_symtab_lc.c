#include "ft_nm.h"

struct symtab_command   *get_symtab_lc(t_arch input)
{
    uint32_t                index;
    struct load_command     *lc;

    lc = input.lc;
    index = 0;
    while( index++ < input.ncmds)
    {
       if (ifswap32(&input, lc->cmd) == LC_SYMTAB) 
           return ((struct symtab_command *)lc);
       if (!(lc = secure_add_mv(input, lc, ifswap32(&input, lc->cmdsize))))
           break ;
    }
    print_error(ERR_MALFORMED, 2);
    return (NULL);
}
