#include "ft_nm.h"

void    swap_mach_header_32(struct mach_header *mh)
{
    mh->magic = ft_swap_int32(mh->magic);
    mh->cputype = ft_swap_int32(mh->cputype);
    mh->cpusubtype = ft_swap_int32(mh->cpusubtype);
    mh->filetype = ft_swap_int32(mh->filetype);
    mh->ncmds = ft_swap_int32(mh->ncmds);
    mh->sizeofcmds = ft_swap_int32(mh->sizeofcmds);
    mh->flags = ft_swap_int32(mh->flags);
}

void    swap_mach_header_64(struct mach_header_64 *mh)
{
    mh->magic = ft_swap_int64(mh->magic);
    mh->cputype = ft_swap_int64(mh->cputype);
    mh->cpusubtype = ft_swap_int64(mh->cpusubtype);
    mh->filetype = ft_swap_int64(mh->filetype);
    mh->ncmds = ft_swap_int64(mh->ncmds);
    mh->sizeofcmds = ft_swap_int64(mh->sizeofcmds);
    mh->flags = ft_swap_int64(mh->flags);
}

void    swap_fat_header(struct fat_header *fh)
{
    fh->magic = ft_swap_int32(fh->magic);
    fh->nfat_arch = ft_swap_int32(fh->nfat_arch);
}
