#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "cdata_ioctl.h"

int main(void)
{
	int fd;
	int i=0;

	/* O_RDWR: read & write 
	   only root can write action in /dev/device 
           but read action is all can do it */	
	fd = open("/dev/cdata", O_RDWR);
	ioctl(fd, CDATA_CLEAR, &i);
	close(fd);
	return 0;
}
