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
      
    #define PORT  8890  
    #define QUEUE_SIZE   10  
    #define BUFFER_SIZE 1024 
    #define REMESS_SIZE 1024

	#define SUPPLY 40  //20*6
	#define CLIENT_NUM 1 
	#define INIT_P0 1000    

	int price[CLIENT_NUM]; //记录最终的各自价格
	int price_var = INIT_P0; //记录动态变化的价格
	double cpu_power[CLIENT_NUM]; //读取到的cpu_power，最终要想得到的各方的cpu_power
	int freq_level[13] = {1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000, 2100000, 2200000, 2300000, 2400000};
	int QoS_min[CLIENT_NUM] = {5}; //各用户的最小freq要求
	int demand[CLIENT_NUM] = {12}; //各用户的当前freq要求，最后发给各方的就是freq_level[demand[j]]
	double sum_d = 0;
	int num_fit[CLIENT_NUM] = {0};
	int client_flag[CLIENT_NUM]; //为处理对应client的返回用
	
	void init_price()
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			price[i] = INIT_P0;
		}
	}
	
	void init_flag()
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			client_flag[i] = 0;
		}
	}
	
	void init_numfit()
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			num_fit[i] = 0;
		}
	}

	void change_demand() //配合情况下，每个都改变
	{
		printf("\nfreq_level[demand[0]]:%d\n", freq_level[demand[0]] ); 
		int i = 0, j = 0;
		int flag[CLIENT_NUM][2];
		init_numfit();   ///////////////////////////////////
		for (i=0; i<CLIENT_NUM; i++)
		{
			flag[i][0] = 0;
			flag[i][1] = 0;
		}
			
		for (i=0; i<12; i++)
		{
			if (price_var > INIT_P0)
			{
				for (j=0; j<CLIENT_NUM; j++)
				{
					if (demand[j] > QoS_min[j])
					{
						demand[j]--;
						if (!(flag[j][0]==1 && flag[j][1]==1))
							num_fit[j]++;
						flag[j][0] = 1;
					}
				}
			}
			else if (price_var < INIT_P0)
			{
				for (j=0; j<CLIENT_NUM; j++)
				{
					if (demand[j] < 12)
					{
						demand[j]++;
						if (!(flag[j][0]==1 && flag[j][1]==1))
							num_fit[j]++;
						flag[j][1] = 1;
					}
				}
			}
			//printf("\nfreq_level[demand[0]]:%d\n", freq_level[demand[0]] ); 
			sum_d = 0;
			for (j=0; j<CLIENT_NUM; j++)
			{
				sum_d = sum_d + cpu_power[j] * (12 + demand[j]) / 24; ///////////////////////
			}
			price_var = INIT_P0 * sum_d / SUPPLY;
			printf("\nprice_var:%d\n", price_var ); 
			
			if (flag[j][0]==1 && flag[j][1]==1)
			{
				if( sum_d > INIT_P0)
				{
					for (j=0; j<CLIENT_NUM; j++)
					{
						if (demand[j] > QoS_min[j])
						{
							demand[j]--;
						}
					}
				}
				break;
			}
			//printf("\ndemand[0]:%d\n", demand[0] ); 
			//printf("\nfreq_level[demand[0]]:%d\n", freq_level[demand[0]] ); 
		}
	}
	
	void allo_freq()
	{
		int j = 0;
		for (j=0; j<CLIENT_NUM; j++)
		{
			sum_d = sum_d + cpu_power[j] * (12 + demand[j]) / 24; ////////////////////////
		}
		// if sum_d <= SUPPLY 按照各自demand分配
		int max_index = 0;
		for (j=0; j<CLIENT_NUM; j++)
		{
			if (cpu_power[j] * (freq_level[demand[j]]) > cpu_power[max_index] * (freq_level[demand[max_index]]))
				max_index = j;
		}
		
		while (sum_d > SUPPLY)
		{
			if (demand[max_index] > 0)
				demand[max_index]--;
			sum_d = sum_d - cpu_power[max_index] / 24;
		}
		//printf("\nfreq_level[demand[0]]:%d\n", freq_level[demand[0]] ); 
	}
	
	void final_price()
	{
		init_price(); ///////////////////////////////////////
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			printf("\nnum_fit[i]:%d\n", num_fit[i] );
			price[i] = price_var - 0.01 * INIT_P0 * num_fit[i]; ////////////////////////////////
		}
	}
	
	void final_print()
	{
		FILE *fp_out;
		int i=0, j=0;
		if ((fp_out = fopen ("/home/hly/server_06/data_out", "a")) == NULL )
		{
			printf("out file cannot be opened\n");
			exit(1);
		}
		
		fputs("cpu power:\n", fp_out);
		for (i=0; i<CLIENT_NUM; i++)
			fprintf(fp_out, "%lf\t", cpu_power[i]);
		
		fputs("\n", fp_out);
		
		fputs("cpu frequency:\n", fp_out);
		for (i=0; i<CLIENT_NUM; i++)
			fprintf(fp_out, "%d\t", (freq_level[demand[i]]));
		
		fputs("\n", fp_out);
		
		fputs("client price:\n", fp_out);
		for (i=0; i<CLIENT_NUM; i++)
			fprintf(fp_out, "%d\t", price[i]);
		
		fputs("\n", fp_out);
		fclose(fp_out);
	}
	
	
    //传进来的sockfd，就是互相建立好连接之后的socket文件描述符  
    //通过这个sockfd，可以完成 [服务端]<--->[客户端] 互相收发数据  
	int p, q;
    void str_echo(int sockfd)  
    {  
        char buffer[BUFFER_SIZE];
		char remess[REMESS_SIZE];  //message to return
		int id; //the id-th client_flag denote this client
		//strcpy(remess, "1300000 \n");
        pid_t pid = getpid();  
        while(1)  
        {  
            memset(buffer,0,sizeof(buffer));  
            int len = recv(sockfd, buffer, sizeof(buffer),0);
			//int relen = 20;
			/*
            if(strcmp(buffer,"exit\n")==0)  
            {  
                printf("child process: %d exited.\n",pid);  
                break;  
            }  
			*/
            printf("\npid:%d && id:0 receive:\n",pid );  
            fputs(buffer, stdout);  
			
			/*
			if(client_flag[0]==client_flag[1]&&client_flag[0]==client_flag[2]&&client_flag[0]==client_flag[3]&&client_flag[0]==client_flag[4]&&client_flag[0]==client_flag[5])
			{
				if (q == 0)
					q ++;
				else if (q>0 && q<7)
				{
					if (q == 1)
					{
						change_demand();
						allo_freq();
						final_price();
					}
					q++;
					sprintf (remess, "%d", freq_level[demand[id]]);
					send(sockfd, remess, sizeof(remess), 0); 
					final_print();
				}
				else if (q == 7)
					q = 1;
			}*/
			change_demand();
			allo_freq();
			final_price();
			sprintf (remess, "%d", (freq_level[demand[0]]));
			printf("\nfreq_level[demand[0]]:%d\n", freq_level[demand[0]] ); 
			send(sockfd, remess, sizeof(remess), 0); 
			final_print();
			//else
				//send(sockfd, buffer, sizeof(buffer), 0);
			
			
			cpu_power[0] = atof(buffer);		
			 
        }  
        close(sockfd);  
    }  
      
    int main(int argc, char **argv)  
    {  
		init_price();
		init_flag();
        //定义IPV4的TCP连接的套接字描述符  
        int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
      
        //定义sockaddr_in  
        struct sockaddr_in server_sockaddr;  
        server_sockaddr.sin_family = AF_INET;  
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
        server_sockaddr.sin_port = htons(PORT);  
      
        //bind成功返回0，出错返回-1  
        if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
        {  
            perror("bind");  
            exit(1);//1为异常退出  
        }  
        printf("bind success.\n");  
      
        //listen成功返回0，出错返回-1，允许同时帧听的连接数为QUEUE_SIZE  
        if(listen(server_sockfd,QUEUE_SIZE) == -1)  
        {  
            perror("listen");  
            exit(1);  
        }  
        printf("listen success.\n");  
      
        for(;;)  
        {  
            struct sockaddr_in client_addr;  
            socklen_t length = sizeof(client_addr);  
            //进程阻塞在accept上，成功返回非负描述字，出错返回-1  
            int conn = accept(server_sockfd, (struct sockaddr*)&client_addr,&length);  
            if(conn<0)  
            {  
                perror("connect");  
                exit(1);  
            }  
            printf("new client accepted.\n");  
      
            pid_t childid;  
            if(childid=fork()==0)//子进程  
            {  
                printf("child process: %d created.\n", getpid());  
                close(server_sockfd);//在子进程中关闭监听  
                str_echo(conn);//处理监听的连接  
                exit(0);  
            }  
        }  
      
        printf("closed.\n");  
        close(server_sockfd);  
        return 0;  
    }  
