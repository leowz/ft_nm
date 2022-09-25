#include "ft_nm.h"

static int		handle_new(t_arch *input, struct fat_arch *arch)
{
	t_arch			*tmp;
	t_handler_func	*handler_funcs;

	handler_funcs = get_nm_flags()->funcs;
	if (!(tmp = (t_arch *)malloc(sizeof(t_arch))))
		return (return_error(ERR_MALLOC, 1));
	tmp->data = secure_add_mv(*input, input->data,
												ifswap32(input, arch->offset));
	tmp->length = ifswap32(input, arch->size);
	tmp->path = input->path;
	if (!tmp->data || !(secure_add(*input, tmp->data, tmp->length)))
		return (return_error(ERR_MALFORMED, 1));
	if (exec_handler(input) == 2)
		return (return_error(ERR_INVALID, 1));
	return (0);
}

static int		handle_fat_archs(t_arch *input, struct fat_header header,
														struct fat_arch *arch)
{
	uint32_t		index;
	const char		*name;
	struct fat_arch	*tmp;

	index = header.nfat_arch;
	tmp = arch;
	while (index--)
	{
		name = get_arch_info_from_cpu_type(ifswap32(input, arch->cputype),
										ifswap32(input, arch->cpusubtype)).name;
		if (!ft_strcmp(name, GET_ARCH))
			return (handle_new(input, arch));
		if (!(arch = secure_add_mv(*input, arch, sizeof(struct fat_arch))))
			return (return_error(ERR_INVALID, 1));
	}
	index = header.nfat_arch;
	arch = tmp;
	while (index--)
	{
		if (handle_new(input, arch))
			return (1);
		if (!(arch = secure_add_mv(*input, arch, sizeof(struct fat_arch))))
			return (return_error(ERR_INVALID, 1));
	}
	return (0);
}

int				handler_fat(t_arch *input)
{
    (void)input;
	/*struct fat_header	header;
	struct fat_arch		*arch;
	int					ret;

	//get_nm_flags()->print_arch = TRUE;
	if (get_header_fat(*input, input->data, &header))
		return (return_error(ERR_INVALID, 2));
	if (!(arch = secure_add_mv(*input, input->data, sizeof(struct fat_header))))
		return (return_error(ERR_INVALID, 2));
	ret = handle_fat_archs(input, header, arch);
	return (ret);
    */
    return (0);
}
