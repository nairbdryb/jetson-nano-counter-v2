import serial
import time
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.1)
def write_read(x):
    data = arduino.readline()
    return data
while True:
    num = input("Enter a number: ") # Taking input from user
    arduino.write(bytes(num, 'utf-8'))
    time.sleep(10)
    for i in range(8):
        value = write_read(num)
        print(value) # printing the value

