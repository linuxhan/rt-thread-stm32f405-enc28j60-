#include <rtthread.h>
#include <lwip/netdb.h> /* ???????,????netdb.h??? */
#include <lwip/sockets.h> /* ??BSD socket,????sockets.h??? */

ALIGN(4)
static const char send_data[] = {0x6F, 0xDE, 0x6F, 0xDE, 0x00};
void tcpclient(const char* url, int port)
{
	char *recv_data;
	struct hostent *host;
	int sock, bytes_received;
	struct sockaddr_in server_addr;
	/* ????????url??host??(?????,??????) */
	host = gethostbyname(url);
	/* ????????????? */
	recv_data = rt_malloc(1024);
	if (recv_data == RT_NULL)
	{
		rt_kprintf("No memory\n");
		return;
	}
	/* ????socket,???SOCKET_STREAM,TCP?? */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		/* ??socket?? */
		rt_kprintf("Socket error\n");
		/* ?????? */
		rt_free(recv_data);
		return;
	}
	/* ???????????? */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *) host->h_addr);
	rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	if (connect(sock, (struct sockaddr *) &server_addr,sizeof(struct sockaddr)) == -1)
	{
	/* ???? */
		rt_kprintf("Connect error\n");
		/*?????? */
		rt_free(recv_data);
		return;
	}
	else
	{
		rt_kprintf("Connect right!\n");
	}
//	while (1)
	send(sock, send_data,5, 0); //strlen(send_data)
	do
	{
		/* ?sock???????1024???? */
		bytes_received = recv(sock, recv_data, 1024, 0);
		if (bytes_received < 0)
		{
			/* ????,?????? */
			lwip_close(sock);
			/* ?????? */
			rt_free(recv_data);
			break;
		}
		/* ??????,????? */
		recv_data[bytes_received] = '\0';
		if (strcmp(recv_data, "q") == 0 || strcmp(recv_data, "Q") == 0)
		{
			/* ???????q?Q,?????? */
			lwip_close(sock);
			/* ?????? */
			rt_free(recv_data);
			break;
		}
		else
		{
			/* ???????????? */
			int i=0;
			rt_kprintf("\nRecieved data ");
			for(i=0;i<bytes_received;i++)
			{
				rt_kprintf("%.2x ", recv_data[i]);
			}
			rt_kprintf("\n");
		}
	}while(0);
	lwip_close(sock);
			/* ?????? */
	rt_free(recv_data);
	return;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
/* ??tcpclient???finsh shell? */
FINSH_FUNCTION_EXPORT(tcpclient, startup tcp client);
#endif

