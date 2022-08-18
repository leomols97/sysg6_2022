#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    offset_t  offset, poffset;
    offset_t  len, plen;
    char  *filename;
    char  errmsg[120];
    int     rc=0;
    int     fd;
    int     i,c;
    int     uflag=0;  /* UNDO Allocation option */
    int     aflag=1;  /* Normal  pre-allocation */
    int     index;
    while ((c = getopt (argc, argv, "dru")) != -1)
        switch (c)
        {
            case 'u':
                uflag = 1;
                aflag=0;
                break;
            case '?':
                if (isprint (optopt))
                {
                    printf( "usage: %s [ -u ] <file_name>  <page_offset> <page_range> \n", argv[0] );
                    printf( " where no option means we pre-allocate range\n");
                    printf( " -u for unallocate range\n");
                }
                else
                    printf ("Unknown option character `\\x%x'.\n", optopt);
                exit ( 1 );
            default:
                printf ("Unknown option character `\\x%x'.\n", optopt);
                exit ( 1 );
        }
    if (( uflag  + aflag) != 1)
    {
        printf("Only one option is allowed exiting\n");
        exit(1);
    }
    if ( ( argc - optind )  !=  3 )
    {
        printf( "usage: %s [ -u ] <file_name>  <page_offset> <page_range> \n", argv[0] );
        printf( " where no option means we pre-allocate range\n");
        printf( " -u for unallocate range\n");
        exit ( 1 );
    }
    i=1;
    for (index = optind; index < argc; index++)
    {
        if (i == 1)
            filename = &argv[index][0];
        else if (i == 2)
            sscanf(argv[index],"%lld", &poffset );
        else if (i == 3)
            sscanf(argv[index],"%lld", &plen );
        i++;
    }
    offset = poffset*PAGESIZE;
    len = plen*PAGESIZE;
    if ( ( fd = open ( filename,O_RDWR) ) < 0 )
    {
        sprintf(errmsg,"%s: cannot open %s ", argv[0], filename);
        perror ( errmsg );
        exit ( 1 );
    }
    
    if ( aflag == 1 )
        rc = posix_fallocate(fd,offset,len);
    else if ( uflag == 1 )
        rc = ftruncate64(fd,offset);
    
    if (rc == -1)
    {
        rc = errno;
        printf(" error: rc:  %d \n",rc);
        return rc;
    }
    return rc;
}
