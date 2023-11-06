import time
import serial
import matplotlib.pyplot as plt
import numpy as np
import sys

def get_args():
    return sys.argv[1], int(sys.argv[2])

def main():
    port, target_rpm = get_args()

    data = []

    serial_port = serial.Serial(port=port, baudrate=115200, bytesize=8, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
    serial_string = ""
    start_time = time.time()
    while start_time + 12 >= time.time():
        if (serial_port.in_waiting > 0):
            serial_string = serial_port.readline()
            data.append(serial_string.decode().strip())

    print(f"Received data: {len(data)}")
    print(data)

    plt.axhline(y=target_rpm, color='r')
    plt.plot(np.array(data))

    plt.show()

if __name__ == "__main__":
    print("Start")
    main()