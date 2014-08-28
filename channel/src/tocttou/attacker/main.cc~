
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
#include <base/printf.h>
int main(int argc, char *argv[])
{
	int ret,fd;
	char const *file_name1 = "/private/private.tst";
	char const *file_name2 = "/public/public.tst";
	fd=open(file_name2,O_RDONLY);
	PERR("open the file: %s",file_name2);
	static char buf[512];
	read(fd, buf, sizeof(buf));
	PERR("content of file: \"%s\"\n", buf);
	close(fd);
	try{
		unlink(file_name2);
		PERR("remove the file:%s",file_name2);
		symlink(file_name1, file_name2);
		PERR("create a link file %s to %s",file_name2,file_name1);
	}catch(...){}
	fd=open(file_name2,O_RDONLY);
	PERR("open the file: %s",file_name2);
	static char buf1[512];
	read(fd, buf1, sizeof(buf1));
	PERR("content of file: \"%s\"\n", buf1);
	close(fd);	
	while(true);
	return 0;
}
