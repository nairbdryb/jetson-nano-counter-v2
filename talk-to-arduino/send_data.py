import serial
import time
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

def write_data(x):
    arduino.write(bytes(x, 'utf-8'))

def read_data():
    data = arduino.readline().decode("utf-8")
    return data


while True:
    num = input("Enter a number: ") # Taking input from user
    write_data(num)
    for i in range(10):
        time.sleep(.3)
        value = read_data() # reading the data from the arduino
        print(value) # printing the value

