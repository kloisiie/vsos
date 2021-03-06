
/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <os/config.h>

/* libc includes */
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <timer_session/client.h>
#include <timer_session/connection.h>
int main(int argc, char *argv[])
{
	int ret, fd;
	Timer::Connection time;
	ssize_t count;
	char const *file_name= "/public/public.tst";
	PERR("begin to test");
	struct stat stat_buf;
	ret=lstat(file_name, &stat_buf);
	PERR("check the state of file:%s",file_name);
	//printf("mode %d",stat_buf.st_mode);
	time.msleep(10000);
	if(ret==0&&!S_ISLNK(stat_buf.st_mode)){
		fd=open(file_name, O_RDONLY);
		PERR("reading the file %s\n",file_name);
		if(fd>=0){
			static char buf[512];
			count=read(fd, buf, sizeof(buf));
			close(fd);
			PERR("content of file: \"%s\"\n", buf);	
		}
	}
	else PERR("it's a link file\n");
	PERR("test finished\n");

	return 0;
}
