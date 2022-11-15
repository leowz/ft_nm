/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:23:00 by zweng             #+#    #+#             */
/*   Updated: 2022/11/13 17:53:08 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int  init_params(t_param *params, int ac, char **av)
{
    int     i;

    i = 1;
    while (i < ac && av[i][0] == '-')
    {

    }
}

int         main(int ac, char **av) {
    int     i;
    int     ret;
    t_param params

    ret = 0;
    i = init_params(&params, ac, av);
    if (ac - i <= 1)
        ret = ft_nm("a.out", params);
    else
    {
        while (i < ac)
        {
           ret += ft_nm(av[i], params);
           i++;
        }
    }
    return (ret);
}
