#simple Python server for raspberry pi used to receive strings from sockets and write the strings via serial to an Arduino

import socket
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=5)
time.sleep(2);
input = ser.readline();
print(input);

soc = socket.socket()
host = "" #IP of local machine
port = 8040
soc.bind((host, port))
soc.listen(5)
while True:
    conn, addr = soc.accept()
    print ("Got connection from",addr)
    msg = conn.recv(1024)
    print (msg)

    # write to Arduino
    ser.write(msg)

    time.sleep(1)

    # read response back from Arduino
    while ser.inWaiting():
         input = ser.readline()
         print ("Read input back: " + input)
