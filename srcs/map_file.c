#include "ft_nm.h"

static int	static_intern_open(const char *prgm, const char *path)
{
	int		fd;

	if ((fd = open(path, O_RDONLY)) < 0)
	{
		ft_dprintf(STDERR_FILENO, ERR_OPEN_FMT, prgm, path);
		return (-1);
	}
    ft_printf("intern open return %d\n", fd);
	return (fd);
}

static int	static_intern_fstat(const char *prgm, int fd, struct stat *buf)
{
	if (fstat(fd, buf) != 0)
	{
		ft_dprintf(STDERR_FILENO, ERR_FSTAT_FMT, prgm, fd);
		return (1);
	}
	if (S_ISDIR(buf->st_mode))
	{
		ft_dprintf(STDERR_FILENO, ERR_IS_DIR_FMT, prgm, fd);
		return (1);
	}
	return (0);
}

static int	static_intern_mmap(const char *prgm, void **data, int fd,
																size_t size)
{
	*data = mmap(FALSE, size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
	if (*data == MAP_FAILED)
	{
		ft_dprintf(STDERR_FILENO, ERR_MMAP_FMT, prgm, fd);
		return (1);
	}
	return (0);
}

static int  close_fd_return(int fd, int ret)
{
	close(fd);
	return (ret);
}

int     map_file(const char *file, t_arch *arch)
{
    struct stat buf;
    void        *data;
    int         fd;

    ft_printf("enter map_file\n");
    data = 0;
    if ((fd = static_intern_open(PROGRAM, file)) < 0)
    {
        ft_printf("static open failed\n");
        return (0);
    }
    if (static_intern_fstat(PROGRAM, fd, &buf))
    {
        ft_printf("fstat failed\n");
		return (close_fd_return(fd, 0));
    }
	if (static_intern_mmap(PROGRAM, &data, fd, buf.st_size))
    {
        ft_printf("mmap failed\n");
		return (close_fd_return(fd, 0));
    }
    if (arch)
	{
		arch->data = data;
		arch->length = buf.st_size;
	}
    ft_printf("return true %d\n", TRUE);
	return (close_fd_return(fd, TRUE));
}
