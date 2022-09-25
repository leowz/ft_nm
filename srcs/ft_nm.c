#include "ft_nm.h"

void    print_help(void)
{
    ft_printf("USAGE: %s <input files>\n", PROGRAM);
}

int     process_file(const char *file)
{
    t_arch      arch;

    if (map_file(file, &arch) && get_magic(&arch))
    {
        ft_printf("magic get %d\n", arch.macho_type);
        ft_printf("ident get %s\n", arch.ident);
        exec_handler(&arch);
        return (EXIT_SUCCESS);
    }
    ft_printf("failed map");
    return (EXIT_FAILURE);
}

int     loop_files(int ac, const char **av)
{
    int     index;
    int     ret;

    index = 1;
    ret = EXIT_SUCCESS;
    while(index < ac)
    {
        if (process_file(av[index]) != EXIT_SUCCESS)
            return (EXIT_FAILURE);
        index++;
    }
    return (EXIT_SUCCESS);
}

int     main(int ac, const char **av)
{
    (void)av;
    if (ac > 1) {
        return loop_files(ac, av);
    } else {
        print_help();
    }
    return (EXIT_SUCCESS);
}
