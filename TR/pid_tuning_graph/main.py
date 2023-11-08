import serial
import matplotlib.pyplot as plt
import numpy as np
import sys

def get_args():
    try:
        return sys.argv[1].strip(), float(sys.argv[2])
    except:
        print("Please provide valid arguments")
        exit()

def main():
    #port, target_rpm = get_args()
    target_rpm = 500.0

    data = []

    data_size = 5000

    serial_port = serial.Serial(port="COM3", baudrate=115200, bytesize=8, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
    serial_string = ""
    while len(data) < data_size:
        if (serial_port.in_waiting > 0):
            serial_string = serial_port.readline()
            try:
                data.append(float(serial_string.decode().strip()))
            except:
                data_size -= 1
            #print(serial_string.decode().strip())

    print(f"Received data: {len(data)}")
    print(data)

    plt.axhline(y=target_rpm, color='r')
    plt.plot(np.array(data))

    plt.show()

if __name__ == "__main__":
    print("Start")
    main()