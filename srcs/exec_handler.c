#include "ft_nm.h"

int     exec_handler(t_arch *arch)
{
    t_type_macho    binary_type;
    Elf32_Ehdr	    header_32;
    Elf64_Ehdr	    header_64;

    ft_printf("enter exec_handler \n");
    binary_type = arch->macho_type;
    if (binary_type == M_32)
    {
        ft_printf("handle 64\n");
        return handler_64(arch, &header_64);
    }
    else if (binary_type == M_64)
    {
        ft_printf("handle 32\n");
        return handler_32(arch, &header_32);
    }
    ft_printf("exec: binary type error: not 32 or 64 \n");
    return (2);
}


