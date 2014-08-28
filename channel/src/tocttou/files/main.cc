
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
#define CALL_AND_CHECK(operation , info_string, ...) \
	printf("calling " #operation " " info_string "\n", ##__VA_ARGS__); 
int main(int argc, char *argv[])
{
	char const *dir_name1 = "/private";
	char const *dir_name2 = "/public";
	char const *file_name1 = "/private/private.tst";
	char const *file_name2 = "/public/public.tst";
	char const *pattern1       = "this is a secret file";
	size_t      pattern_size1  = strlen(pattern1) + 1;
	char const *pattern2       = "this is a public file";
	size_t      pattern_size2  = strlen(pattern2) + 1;
	int fd,ret,count;
	mkdir(dir_name1, 0777);
	CALL_AND_CHECK(mkdir, "dir_name=%s", dir_name1);
	
	mkdir(dir_name2, 0777);
	CALL_AND_CHECK(mkdir, "dir_name=%s", dir_name2);
	
	fd=open(file_name1, O_CREAT | O_WRONLY);
	CALL_AND_CHECK(create_file, "file_name=%s", file_name1);
	
	write(fd, pattern1, pattern_size1);
	CALL_AND_CHECK(write_file, "file:%s  content:%s",file_name1,pattern1);
	
	close(fd);	
	
	fd=open(file_name2, O_CREAT | O_WRONLY);
	CALL_AND_CHECK(create_file, "file_name=%s", file_name2);
	write(fd, pattern2, pattern_size2);
	CALL_AND_CHECK(write_file, "file:%s  content:%s",file_name2,pattern2);
	close(fd);	
	return 0;
}
