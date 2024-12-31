import serial
import time

# -Configuration-
port = '/dev/ttyACM0'  
baud_rate = 9600       
ard = serial.Serial(port, baud_rate, timeout=1)

# Read data from Arduino
def read_data():
    data = ard.readline()  
    return data.decode('utf-8').strip() 

# Main loop to continuously read data
if __name__ == "__main__":
    try:
        print("Reading data from Arduino")
        while True:
            data = read_data()
            if data:  # Print if data output from arduino 
                print(f"Arduino says: {data}")
            time.sleep(0.1)  # delay 
    finally:
        ard.close()
        print("Arduino connection closed.")