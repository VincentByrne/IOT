import serial
import time
from sense_hat import SenseHat
import BlynkLib

# --- Project file imports ---
from capture_image import capture_image
from upload_image import upload_image
from shakey import is_shaken

# --- Arduino Configuration ---
port = '/dev/ttyACM0' 
baud_rate = 9600
ard = serial.Serial(port, baud_rate, timeout=1)

def read_data():
    data = ard.readline()
    return data.decode('utf-8').strip()

# --- Sense HAT Color Setup ---
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
sense = SenseHat()
sense.clear(GREEN)

# --- Blynk Setup ---
BLYNK_AUTH = 'KgE35mx5JhqgzW1n_im5zOxq4ARybSF2'
IMAGE_PATH = "./images/sensehat_image.jpg"
blynk = BlynkLib.Blynk(BLYNK_AUTH)

# --- Blynk Virtual Pins ---
# V0 = Temperature display
# V1 = Switch
# V2 = Gallery
# V3 = shake/movement indicator

@blynk.on("V1")
def handle_v1_write(value):
    button_value = value[0]
    print(f"Current button value: {button_value}")

    if button_value == "1":
        # Turn Sense HAT white
        sense.clear(255, 255, 255)
        # Capture and upload an image - https://button-coral-quicksand.glitch.me/
        capture_image(IMAGE_PATH)
        result_url = upload_image(IMAGE_PATH)
        blynk.set_property(2, "urls", result_url)
    else:
        sense.clear(GREEN)

if __name__ == "__main__":
    print("Listening for Blynk events and Arduino")
    try:
        while True:
            blynk.run()
            # Read Arduino data
            data = read_data()
            if data:
                # Check if door opened and send blynk event
                print(f"Arduino says: {data}")
                # Door opened event
                if "Door Opened!" in data:
                    print("Door event detected")
                    blynk.log_event("door_event")
                    sense.clear(BLUE)
                    time.sleep(0.5)
                    sense.clear(GREEN)
                # Access granted event - take and send picture
                if "Access Granted." in data:
                    print("Access event detected")
                    blynk.log_event("access_event")
                    capture_image(IMAGE_PATH)
                    result_url = upload_image(IMAGE_PATH)
                    blynk.set_property(2, "urls", result_url)
                    sense.clear(YELLOW)
                    time.sleep(5)
                    sense.clear(GREEN)
                # Alarm triggered event - take and send picture
                if "Maximum attempts exceeded. Alarm Activated!" in data:
                    print("alarm event detected")
                    blynk.log_event("alarm_event")
                    capture_image(IMAGE_PATH)
                    result_url = upload_image(IMAGE_PATH)
                    blynk.set_property(2, "urls", result_url)
                    sense.clear(RED)
            
            # Check if the Sense_hat was shaken
            if is_shaken():
                print("Security device moved.")
                blynk.log_event("movement_event")  # Log event in Blynk
                sense.clear(RED)
                blynk.virtual_write(3, 1)  # Movement on V3
                time.sleep(2) 
                sense.clear(GREEN)
                blynk.virtual_write(3, 0)

            blynk.virtual_write(0, round(sense.temperature, 2)) # temperature on V0
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\nBlynk and Arduino stopped.")
