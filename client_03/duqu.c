#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "change_cpu.h"

int main()
{
	FILE *fp;
	char turbo_power [10];
	int i = 260, j = 0;
	
	system ("sudo sh -c 'timeout 5 turbostat --debug -S -i 2 > turbo_out'");
	sleep(2);
	
	if ((fp = fopen ("/home/hly/turbo_out", "r")) == NULL )
	{
		printf("turbo_out file cannot be opened\n");
		exit(1);
	}
	while(i--)
	{
		fgetc(fp);
		//strcat(turbo_power, "ss");
	}
	while(j < 6)
	{
		turbo_power[j] = fgetc(fp);
		//strcat(turbo_power, fgetc(fp));
		j++;
	}
	
	printf ("\n");
	printf ("%s\n", turbo_power);
	fclose (fp);
	
	change_cpu("1300000");
	change_cpu("1300000");

	return 0;
}
