﻿    #include <sys/types.h>  
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

	#define SUPPLY 20  //
	#define CLIENT_NUM 4
	#define INIT_P0 1000    

	int price[CLIENT_NUM]; //记录最终的各自价格
	int price_var = INIT_P0; //记录动态变化的价格
	double cpu_power[CLIENT_NUM]; //读取到的cpu_power，最终要想得到的各方的cpu_power
	double cpu_power_default[CLIENT_NUM] = {20, 20, 9, 21.5};
	int freq_level[13] = {1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000, 2100000, 2200000, 2300000, 2400000};
	int QoS_min[CLIENT_NUM] = {9, 9, 9, 9}; //各用户的最小freq要求
	int demand[CLIENT_NUM] = {12, 12, 12, 12}; //各用户的当前freq要求，最后发给各方的就是freq_level[demand[j]]
	double sum_d = 0;
	int num_fit[CLIENT_NUM] = {0, 0, 0, 0};
	//int client_flag[CLIENT_NUM]; //为处理对应client的返回用
	
	void init_price() //set price to INIT_P0
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			price[i] = INIT_P0;
		}
	}
	
	/*
	void init_flag()
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			client_flag[i] = 0;
		}
	}*/

	void init_numfit() //set the number of cooperation to 0
	{
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			num_fit[i] = 0;
		}
	}
	
	void init_file() //delete the history of previous results
	{
		FILE *fp_init;
		if ((fp_init = fopen ("/home/hly/server_06/data_out", "w")) == NULL )
		{
			printf("init data_out file cannot be opened\n");
			exit(1);
		}
		fclose(fp_init);
		
		if ((fp_init = fopen ("/home/hly/server_06/tmp/id_tmp", "w")) == NULL )
		{
			printf("init id_tmp file cannot be opened\n");
			exit(1);
		}
		fprintf(fp_init, "%d\t", 0);
		fclose(fp_init);
		
		if ((fp_init = fopen ("/home/hly/server_06/tmp/price_var", "w")) == NULL )
		{
			printf("init price_var file cannot be opened\n");
			exit(1);
		}
		fprintf(fp_init, "%d\t", 1000);
		fclose(fp_init);
		
		if ((fp_init = fopen ("/home/hly/server_06/tmp/demand", "w")) == NULL )
		{
			printf("init demand file cannot be opened\n");
			exit(1);
		}
		fprintf(fp_init, "%d\t%d\t%d\t%d", 12, 12, 12, 12);
		fclose(fp_init);
		
		if ((fp_init = fopen ("/home/hly/server_06/tmp/cpu_power", "w")) == NULL )
		{
			printf("init cpu_power file cannot be opened\n");
			exit(1);
		}
		fprintf(fp_init, "%d\t%d\t%d\t%d", 1, 1, 1, 1);
		fclose(fp_init);
	}

	void change_demand() //change the demands of clients, with the guarantee of QoS
	{
		int i = 0, j = 0;
		int flag[CLIENT_NUM][2];
		init_numfit();
		for (i=0; i<CLIENT_NUM; i++)
		{
			flag[i][0] = 0;
			flag[i][1] = 0;
		}
			
		for (i=0; i<10; i++)
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
			sum_d = 0;
			for (j=0; j<CLIENT_NUM; j++)
			{
				sum_d = sum_d + cpu_power[j] * (12 + demand[j]) / 24;
			}
			price_var = INIT_P0 * sum_d / SUPPLY;
			
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
		}
	}
	
	void allo_freq() //allocate CPU frequency to each client
	{
		int j = 0;
		for (j=0; j<CLIENT_NUM; j++)
		{
			sum_d = sum_d + cpu_power[j] * (12 + demand[j]) / 24;
		}
		// if sum_d <= SUPPLY 按照各自demand分配
		int max_index = 0;
		for (j=0; j<CLIENT_NUM; j++)
		{
			if (cpu_power[j] * freq_level[demand[j]] > cpu_power[max_index] * freq_level[demand[max_index]])
				max_index = j;
		}
		
		while (sum_d > SUPPLY)
		{
			if (demand[max_index] > 0)
				demand[max_index]--;
			sum_d = sum_d - cpu_power[max_index] / 24;
		}
	}
	
	void final_price() //calculate the final price
	{
		init_price(); //如果不重置，可能一个一直在调一直在配合，最后钱变负值 12307
		int i = 0;
		for (i=0; i<CLIENT_NUM; i++)
		{
			price[i] = price_var - 0.02 * INIT_P0 * num_fit[i];
		}
	}
	
	void final_print() //print related statistic
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
			fprintf(fp_out, "%d\t", freq_level[demand[i]]);
		
		fputs("\n", fp_out);
		
		fputs("client price:\n", fp_out);
		for (i=0; i<CLIENT_NUM; i++)
			fprintf(fp_out, "%d\t", price[i]);
		
		fputs("\n", fp_out);
		fputs("\n", fp_out);
		fclose(fp_out);
	}
	
	
    //传进来的sockfd，就是互相建立好连接之后的socket文件描述符  
    //通过这个sockfd，可以完成 [服务端]<--->[客户端] 互相收发数据  
	int p=0, q=0;
	
    void str_echo(int sockfd) //address the received data (CPU power of each client) 
    {  
        char buffer[BUFFER_SIZE];
		char remess[REMESS_SIZE];  //message to return
		int id; //the id-th client_flag denote this client
		//strcpy(remess, "2400000");
		FILE *fp_pidr, *fp_pidw, *fp_r, *fp_w;
		
		
        pid_t pid = getpid();  
        while(1)  
        {  
			if ((fp_pidr = fopen ("/home/hly/server_06/tmp/id_tmp", "r")) == NULL )
			{
				printf("id_tmp file cannot be opened\n");
				exit(1);
			}
			
            memset(buffer,0,sizeof(buffer));  
            int len = recv(sockfd, buffer, sizeof(buffer),0);
			//id = getw(fp_pidr);
			fscanf(fp_pidr, "%d ", &id);
			fclose(fp_pidr);
			int idw = (id + 1) % 4;
			printf("\nid=%d\n", id);
			//putw(id, fp_pidw);
			if ((fp_pidw = fopen ("/home/hly/server_06/tmp/id_tmp", "w")) == NULL )
			{
				printf("id_tmp file cannot be opened\n");
				exit(1);
			}
			fprintf(fp_pidw, "%d ", idw);
			fclose(fp_pidw);
			
			if ((fp_r = fopen ("/home/hly/server_06/tmp/cpu_power", "r")) == NULL )
			{
				printf("cpu_power file cannot be opened\n");
				exit(1);
			}
			for (p=0; p<CLIENT_NUM; p++)
			{
				fscanf(fp_r, "%lf", &cpu_power[p]);
				//cpu_power[p] = getw(fp_r);
			}
			fclose(fp_r);
			
			if ((fp_r = fopen ("/home/hly/server_06/tmp/demand", "r")) == NULL )
			{
				printf("demand file cannot be opened\n");
				exit(1);
			}
			for (p=0; p<CLIENT_NUM; p++)
			{
				fscanf(fp_r, "%d ", &demand[p]);
				//demand[p] = getw(fp_r);
			}
			fclose(fp_r);
			for (p=0; p<CLIENT_NUM; p++)
			{
				printf("\ndemand[p]=%d\n",demand[p]);  
			}
			if ((fp_r = fopen ("/home/hly/server_06/tmp/price_var", "r")) == NULL )
			{
				printf("price_var file cannot be opened\n");
				exit(1);
			}
			fscanf(fp_r, "%d ", &price_var);
			fclose(fp_r);
			
			
			printf("\npid:%d && id:%d receive:\n",pid, id);  
            fputs(buffer, stdout);
			cpu_power[id] = atof(buffer) - cpu_power_default[id];
			printf("\ncpu_power[id]:%lf\n",cpu_power[id]);	

			if ((fp_w = fopen ("/home/hly/server_06/tmp/cpu_power", "w")) == NULL )
			{
				printf("cpu_power file cannot be opened\n");
				exit(1);
			}
			for (p=0; p<CLIENT_NUM; p++)
			{
				fprintf(fp_w, "%f ", cpu_power[p]);
				//putw(cpu_power[p], fp_w);
			}
			fclose(fp_w);			
				
			if (id == 3)
			{
				change_demand();
				allo_freq();
				final_price();				
			}			
			
			if ((fp_w = fopen ("/home/hly/server_06/tmp/demand", "w")) == NULL )
			{
				printf("demand file cannot be opened\n");
				exit(1);
			}
			for (p=0; p<CLIENT_NUM; p++)
			{
				fprintf(fp_w, "%d ", demand[p]);
				//putw(demand[p], fp_w);
			}
			fclose(fp_w);
			
			if ((fp_w = fopen ("/home/hly/server_06/tmp/price_var", "w")) == NULL )
			{
				printf("price_var file cannot be opened\n");
				exit(1);
			}
			fprintf(fp_w, "%d ", price_var);
			fclose(fp_w);
			
			sprintf (remess, "%d", freq_level[demand[id]]);
			send(sockfd, remess, sizeof(remess), 0); 
			printf("\nsend to id=%d\n",id);
			if (id == 3)
				final_print();
        }  
        close(sockfd);  
    }  
      
    int main(int argc, char **argv)  
    {  
		init_price();
		//init_flag();
		init_file();
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
