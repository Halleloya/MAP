	#include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>   
    #include "change_cpu.h"
	
    
    int main()  
    {  
		
		char turbo_power [10];

        //客户端将控制台输入的信息发送给服务器端，服务器原样返回信息  
		change_cpu("2400000");
		change_cpu("2400000");
        while (1)//(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)  
        {  
			FILE *fp;
			
	
			system ("sudo sh -c 'timeout 5 turbostat --debug -S -i 2 > turbo_out'");
			sleep(2);
			
			if ((fp = fopen ("/home/hly/client_02/turbo_out", "r")) == NULL )
			{
				printf("turbo_out file cannot be opened\n");
				exit(1);
			}
			int i = 212;
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
			fclose (fp);
			
			
			if ((fp = fopen ("/home/hly/client_02/origin_power_out", "a")) == NULL )
			{
				printf("origin_power_out file cannot be opened\n");
				exit(1);
			}
			fprintf (fp, "%s\n", turbo_power);
			fclose (fp);
			
			
			sleep(2);
        }  
        return 0;  
    }  



