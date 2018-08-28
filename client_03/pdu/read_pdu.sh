#!/bin/bash

htmlFile="port_status.shtml"
outputFile="read_pdu.txt"

#for((integer = 1; integer <= 2; integer++))  
while true
do 
	starttime=`date +'%Y-%m-%d %H:%M:%S'`

	if [ -f "$htmlFile" ]; 
	then
		echo 'deleting', $htmlFile
		rm -rf $htmlFile
	fi 

	if [ -f "$outputFile" ]; 
        then
                echo 'deleting', $outputFile
                #rm -rf $outputFile
        fi

	`wget --user=user --password=user http://192.168.1.100/port_status.shtml`
	x=`cat port_status.shtml | grep CA`
	echo $x>read_pdu.txt

#	sleep 1s

	endtime=`date +'%Y-%m-%d %H:%M:%S'`
	start_seconds=$(date --date="$starttime" +%s);
	end_seconds=$(date --date="$endtime" +%s);
	echo "本次运行时间： "$((end_seconds-start_seconds))"s"

done



