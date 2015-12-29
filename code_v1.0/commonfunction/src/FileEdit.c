#include <rtthread.h>
#ifdef RT_USING_DFS
#include <dfs_fs.h>
#endif
#include <dfs_posix.h>

static rt_int8_t checkpath(const char* path)
{
	DIR* dirpath;
	rt_int8_t ret;
	
	dirpath=opendir(path);
	if(RT_NULL==dirpath)
	{
//		rt_kprintf("The path isn't exist!\n");
		ret=-1;
	}
	else
	{
		ret=0;
	}
	closedir(dirpath);
	return ret;
}

rt_int8_t create_file(const char* path,const char* filename)
{
	rt_int8_t ret;
	
	if(checkpath(path))
	{
		rt_kprintf("The path isn't exist!\n");
		ret=-1;
	}
	else
	{
		int fd=0;
		rt_int8_t len=strlen(path)+strlen(filename)+2;
		char* filemem=rt_malloc(len);
		
		strcpy(filemem,path);
		strcpy(filemem,"\\");
		strcat(filemem,filename);
		
		fd=open(filemem,O_RDONLY,0);
		if(fd<0)
		{
			fd=open(filemem,O_CREAT,0);
			if(fd<0)
			{
				rt_kprintf("create file erro!\n");
				ret=-3;
			}
			else
			{
				close(fd);
			}
		}
		else
		{
			rt_kprintf("The file is already exist!\n");
			ret=-2;
		}
	}

	return ret;
}

rt_int8_t Read_file(const char* path,const char* filename,rt_int8_t* buf,rt_int16_t len)
{
	rt_int8_t ret;
	
	if(checkpath(path))
	{
		rt_kprintf("The path isn't exist!\n");
		ret=-1;
	}
	else
	{
		int fd=0;
		rt_int8_t filelen=strlen(path)+strlen(filename)+2;
		char* filemem=rt_malloc(filelen);
		
		strcpy(filemem,path);
		strcpy(filemem,"\\");
		strcat(filemem,filename);
		fd=open(filemem,O_RDONLY,0);
		if(fd<0)
		{
			rt_kprintf("file isn't exist!\n");
		}
		else
		{
			ret=read(fd,buf,len);
		}
	}
	return ret;
}

rt_int8_t Write_file(const char* path,const char* filename,rt_int8_t* buf,rt_int16_t len)
{
	rt_int8_t ret;
	if(checkpath(path))
	{
		rt_kprintf("The path isn't exist!\n");
		ret=-1;
	}
	else
	{
		int fd=0;
		rt_int8_t filelen=strlen(path)+strlen(filename)+2;
		char* filemem=rt_malloc(filelen);
		
		strcpy(filemem,path);
		strcpy(filemem,"\\");
		strcat(filemem,filename);
		fd=open(filemem,O_RDONLY,0);
		if(fd<0)
		{
			rt_kprintf("file isn't exist!\n");
		}
		else
		{
			ret=read(fd,buf,len);
		}
	}
	return ret;
}

void file_test(void)
{
		int fd=0;

		fd =open("/my_dir/hello.txt",DFS_O_CREAT|DFS_O_RDWR,0);
		if(fd <0)
		{
				rt_kprintf("open file failed!\r\n");
		}
		else
		{
				int count =write(fd,"123456",7);
				char buf[10];

				close(fd);
				fd =0;


				rt_thread_delay(50);
				rt_memset(buf,0,10);
				fd =open("/my_dir/hello.txt",DFS_O_RDONLY,0);
				if(read(fd,buf,7))
				{
						rt_kprintf("read=%s\r\n",buf);
				}
				else
				{
						rt_kprintf("read file err!\r\n");
				}

		}
}
