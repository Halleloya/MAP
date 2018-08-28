from socket import *
from select import *
import time
addr = ('192.168.1.101',5025) # PA310

while True:
	s = socket(AF_INET, SOCK_STREAM)
	s.connect(addr) # connect
	output = open('pwhile_out', 'w')
	s.send(b':NUMERIC:HOLD ON\n')
	s.send(b':NUMERIC:ITEM1 I,1\n')
	s.send(b':NUMERIC:VALUE?\n') # command
	s.send(b':NUMERIC:HOLD OFF\n')
	r = s.recv(10) # response
	print(r)
	output.write(r+'\n')
	output.close()
	s.close() # disconnect
	time.sleep(1)
