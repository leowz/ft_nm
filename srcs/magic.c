#include "ft_nm.h"

int         check_magic(t_arch *input)
{
    if (input->ident && input->ident[EI_MAG0] == ELFMAG0 &&
            input->ident[EI_MAG1] == ELFMAG1 && input->ident[EI_MAG2 ==
            ELFMAG2 && input->ident[EI_MAG3 == ELFMAG3]])
    {
        return (TRUE);
    }
    ft_printf("file format not correct\n");
    return (FALSE);
}

int        set_macho_type(t_arch *input)
{
    if (input->ident)
    {
        ft_printf("macho number: %d\n", input->ident[EI_CLASS]);
        if (input->ident[EI_CLASS] == ELFCLASS32)
            input->macho_type = M_32;
        else if (input->ident[EI_CLASS] == ELFCLASS64)
            input->macho_type = M_64;
        else
        {
            return (FALSE);
        }
        return (TRUE);
    }
    return (FALSE);
}

int        set_endian(t_arch *input)
{
    if (input->ident)
    {
        if (input->ident[EI_DATA] == ELFDATA2MSB)
            input->endian_type = BIG;
        else if (input->ident[EI_DATA] == ELFDATA2LSB)
            input->endian_type = LITTLE;
        else
        {
            return (FALSE);
        }
        return (TRUE);
    }
    return (FALSE);
}

uint32_t	get_magic(t_arch *input)
{
    ft_printf("enter get magic\n");
	if (secure_add(*input, input->data, sizeof(unsigned char) * EI_NIDENT))
	{
		input->ident = (unsigned char *)input->data;
        if (!check_magic(input))
        {
            ft_printf("get_magic: return true  %d\n", TRUE);
            return (FALSE);
        }
        if (!set_macho_type(input) || !set_endian(input))
        {
            return (FALSE);
        }
        ft_printf("get_magic: return true  %d\n", TRUE);
		return (TRUE);
	}
    ft_printf("get magic return false\n");
	return (FALSE);
}



