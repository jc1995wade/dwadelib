#coding=utf-8
#-*- coding: UTF-8 -*
import socket
import thread
import time
import datetime


# no use
def ttest():
	#def getIP(domain):
	dest_ip = socket.getaddrinfo("card.gpslink.cn", 'http')
	print(dest_ip)
	lengh = len(dest_ip)
	print(lengh)
	print(dest_ip[0][4][0])

def get_logcal_time():
	now_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	print(now_time)
	return now_time
	#time1 = datetime.datetime.strptime(now_time,'%Y-%m-%d %H:%M:%S')
	#print(time1)



def data_receive_fun(threadName):
	print(threadName+"  start ...\n")
	while True:
		print("data:")
		time.sleep(1)

def data_receive_thread_start():
	try:
		thread.start_new_thread(data_receive_fun, ("Thread-1", ) )
	except:
		print "Error: unable to start thread"

def main():
	#def getIP(domain):
	dest_ip = socket.getaddrinfo("card.gpslink.cn", 'http')
	print(dest_ip)
	lengh = len(dest_ip)
	print(lengh)
	print(dest_ip[0][4][0])

	tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


	dest_port = int(9001)
	dest_addr = (dest_ip[0][4][0], dest_port)
	print(dest_addr)

	tcp_socket.connect(dest_addr)


	send_data = "["+get_logcal_time()+",A,RH1,355889008722099,T1,13510915841,15900622429,123456,460029006600946,861842030011616,RHBD]"
	print(send_data)
	tcp_socket.send(send_data)    


	recv_data = tcp_socket.recv(2048)
	print(recv_data.decode("utf-8"))    

	tcp_socket.close()
	data_receive_thread_start()
	while True:
		print("data:main")
		time.sleep(2)

if __name__ == "__main__":
	main()