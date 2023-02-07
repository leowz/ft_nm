/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:23:00 by zweng             #+#    #+#             */
/*   Updated: 2023/02/07 15:40:46 by vagrant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <string.h>
#include <errno.h>

static int  is_arg(const char *str)
{
    return (!ft_strncmp(str, "-a\0", 3) || !ft_strncmp(str, "-g\0", 3) || 
        !ft_strncmp(str, "-u\0", 3) || !ft_strncmp(str, "-r\0", 3) || 
        !ft_strncmp(str, "-p\0", 3));
}

static void print_params(t_param params)
{
    ft_printf("params: \n");
    ft_printf("-a: %d\n", (params & ARG_A) > 0);
    ft_printf("-g: %d\n", (params & ARG_G) > 0);
    ft_printf("-u: %d\n", (params & ARG_U) > 0);
    ft_printf("-r: %d\n", (params & ARG_R) > 0);
    ft_printf("-p: %d\n", (params & ARG_P) > 0);
    ft_printf("%d\n", params);
}

static int  init_params(t_param *params, int ac, char **av)
{
    int     i;
    int     file_count;

    i = 1;
    file_count = 0;
    while (i < ac)
    {
        if (!ft_strncmp(av[i], "-a\0", 3))
            *params |= ARG_A;
        else if (!ft_strncmp(av[i], "-g\0", 3))
            *params |= ARG_G;
        else if (!ft_strncmp(av[i], "-u\0", 3))
            *params |= ARG_U;
        else if (!ft_strncmp(av[i], "-r\0", 3))
            *params |= ARG_R;
        else if (!ft_strncmp(av[i], "-p\0", 3))
            *params |= ARG_P;
        else
            file_count++;
        i++;
    }
    return (file_count);
}

static int  ft_nm(const char *filename, t_param params)
{
    int             fd;
    int             ret;
    unsigned char   arch, endian;
    struct stat     statbuf;
    void            *file;

    if((fd = open(filename, O_RDONLY)) == -1)
        return error_msg("No Such file\n");
    if (fstat(fd, &statbuf) == -1)
        return error_msg_cleanup("fstat get file size failed\n", fd, NULL, 0);
    if (statbuf.st_size < 16)
        return error_msg_cleanup("file size too small\n", fd, NULL, 0);
    if ((file = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
            == MAP_FAILED)
    {
        return error_msg_cleanup("mmap failed\n", fd, NULL, 0);
    }
    if (!check_elf_ident(file, &arch, statbuf.st_size))
        return error_msg_cleanup("checked elf ident failed\n",
            fd, file, statbuf.st_size);
    if (arch == ELFCLASS32)
        ret = ft_nm32(file, statbuf.st_size, params);
    else if (arch == ELFCLASS64)
        ret = ft_nm64(file, statbuf.st_size, params);
    else
        ret = error_msg_cleanup("arch get failed\n", fd, file,
                statbuf.st_size);
    return (ret);
}

int         main(int ac, char **av) {
    int     i;
    int     file_count;
    int     ret;
    t_param params;

    ret = 0;
    i = 1;
    ft_bzero(&params, sizeof(t_param));
    file_count = init_params(&params, ac, av);
    if (file_count < 1)
        ret = ft_nm("a.out", params);
    else
    {
        while (i < ac)
        {
            if (!is_arg(av[i]))
            {
                if (file_count > 1)
                    ft_printf("\n%s: \n", av[i]);
                ret += FUN_EXT(ft_nm(av[i], params));
            }
            i++;
        }
    }
    return (ret);
}
