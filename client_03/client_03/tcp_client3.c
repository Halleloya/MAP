	#include <sys/types.h>  
    #include <sys/socket.h>  
    #include <stdio.h>  
    #include <netinet/in.h>  
    #include <arpa/inet.h>  
    #include <unistd.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <fcntl.h>  
    #include <sys/shm.h>  
    #include "change_cpu.h"
	
    #define PORT  8890  
    #define BUFFER_SIZE 1024  
    
    //int rec_freq;
    //int cur_freq;
	
    int main(int argc, char **argv)  
    {  
		
		char turbo_power [10];
		
        if(argc!=2)  
        {  
            printf("usage: client IP \n");  
            exit(0);  
        }  
      
        //定义IPV4的TCP连接的套接字描述符  
        int sock_cli = socket(AF_INET,SOCK_STREAM, 0);  
      
        //定义sockaddr_in  
        struct sockaddr_in servaddr;  
        memset(&servaddr, 0, sizeof(servaddr));  
        servaddr.sin_family = AF_INET;  
        servaddr.sin_addr.s_addr = inet_addr(argv[1]);  
        servaddr.sin_port = htons(PORT);  //服务器端口  
      
        //连接服务器，成功返回0，错误返回-1  
        if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)  
        {  
            perror("connect");  
            exit(1);  
        }  
        printf("connect server(IP:%s).\n",argv[1]);  
      
        char sendbuf[BUFFER_SIZE];  
        char recvbuf[BUFFER_SIZE];  
        //客户端将控制台输入的信息发送给服务器端，服务器原样返回信息  
        while (1)//(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)  
        {  
			//char *freq = freq_rec;
			//char *freq_min = "1500000";
			FILE *fp, *fp_freq;
			
			if ((fp_freq = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r")) == NULL )
			{
				printf("cpuinfo_cur_freq file cannot be opened\n");
				exit(1);
			}
			int j = 0;
			while(j < 7)
			{
				sendbuf[j] = fgetc(fp_freq);
				j++;
			}
			
            send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送  
            if(strcmp(sendbuf,"exit\n")==0)  
            {  
                printf("client exited.\n");  
                break;  
            }  
			sleep(5);
            printf("client receive:\n");  
            recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收  
            fputs(recvbuf, stdout);  
			//rec_freq = atoi(recvbuf);
			//printf("it is %d\n", rec_freq);
		
			
	
			system ("sudo sh -c 'timeout 5 turbostat --debug -S -i 2 > turbo_out'");
			sleep(2);
			
			if ((fp = fopen ("/home/hly/client_sail03/turbo_out", "r")) == NULL )
			{
				printf("turbo_out file cannot be opened\n");
				exit(1);
			}
			int i = 260;
			int k = 0;
			while(i--)
			{
				fgetc(fp);
				//strcat(turbo_power, "ss");
			}
			while(k < 6)
			{
				turbo_power[k] = fgetc(fp);
				//strcat(turbo_power, fgetc(fp));
				k++;
			}
			send(sock_cli, turbo_power, strlen(turbo_power),0); ///发送
			
			printf ("\n");
			printf ("%s\n", turbo_power);
			fclose (fp);
			
			change_cpu(recvbuf);
			change_cpu(recvbuf);
	          
            memset(sendbuf, 0, sizeof(sendbuf));  
            memset(recvbuf, 0, sizeof(recvbuf));  
        }  
      
        close(sock_cli);  
        return 0;  
    }  



