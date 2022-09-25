#include "ft_nm.h"

uint16_t    ft_swap_int16(uint16_t x)
{
    return (((x & 0xff00) >> 8) | ((x & 0x00ff) << 8));
}

uint32_t    ft_swap_int32(uint32_t x)
{
    return (((x & 0xff000000) >> 24)    | 
            ((x & 0x00ff0000) >> 8)     |
            ((x & 0x0000ff00) << 8)     |
            ((x & 0x000000ff) << 24));
}

uint64_t    ft_swap_int64(uint64_t x)
{
    return (((x & 0xff00000000000000ULL) >> 56) |
            ((x & 0x00ff000000000000ULL) >> 40) |
            ((x & 0x0000ff0000000000ULL) >> 24) |
            ((x & 0x000000ff00000000ULL) >> 8)  |
            ((x & 0x00000000ff000000ULL) << 8)  |
            ((x & 0x0000000000ff0000ULL) << 24) |
            ((x & 0x000000000000ff00ULL) << 40) |
            ((x & 0x00000000000000ffULL) << 56));
}

void        swap_symtab_command(struct symtab_command *st)
{
    st->cmd = ft_swap_int32(st->cmd);
	st->cmdsize = ft_swap_int32(st->cmdsize);
	st->symoff = ft_swap_int32(st->symoff);
	st->nsyms = ft_swap_int32(st->nsyms);
	st->stroff = ft_swap_int32(st->stroff);
	st->strsize = ft_swap_int32(st->strsize);
}
