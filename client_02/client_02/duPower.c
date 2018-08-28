#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *fp;
	char turbo_power [10];
	int i = 212, j = 0;
	if ((fp = fopen ("/home/hly/client_02/turbo_out", "r")) == NULL )
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
	return 0;
}
