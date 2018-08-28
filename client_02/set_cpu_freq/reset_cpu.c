#include <stdio.h>
#include <stdlib.h>

void main()
{
	FILE *freq_new;
	char *freq = "1200000";
	//char *freq_min = "1500000";

	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu0 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu0 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu1 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu1 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu2 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu2 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu3 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu3 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu4 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu4 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu5 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu5 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu6/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu6 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu6 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu7/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu7 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
		
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu7 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu8/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu8 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu8/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu8 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu9/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu9 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu9/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu9 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu10/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu10 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu10/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu10 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu11/cpufreq/scaling_max_freq", "w")) == NULL )
	{
		printf("cpu11 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	if ((freq_new = fopen ("/sys/devices/system/cpu/cpu11/cpufreq/scaling_min_freq", "w")) == NULL )
	{
		printf("cpu11 file cannot be opened\n");
		exit(1);
	}
	fprintf(freq_new, "%s", freq);
	
	fcloseall();
}