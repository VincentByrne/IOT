import BlynkLib
import time
from time import sleep
from sense_hat import SenseHat

# Imports from other project files for image capture, upload, and movement
from capture_image import capture_image
from upload_image import upload_image
from shakey import is_shaken

# Colour configuration 
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# Initalise sense_hat 
sense = SenseHat()
sense.clear(GREEN)

# Blynk authentication token
BLYNK_AUTH = 'KgE35mx5JhqgzW1n_im5zOxq4ARybSF2'
IMAGE_PATH = "./images/sensehat_image.jpg"
blynk = BlynkLib.Blynk(BLYNK_AUTH)

# Register handler for virtual pin V1 write event
@blynk.on("V1")
def handle_v1_write(value):
    button_value = value[0]
    print(f'Current button value: {button_value}')
    
    if button_value=="1":
        sense.clear(255,255,255)
        capture_image(IMAGE_PATH)
        result = upload_image(IMAGE_PATH)
        
        blynk.set_property(2,"urls",result) #updates ulrs property of widget attached to Datastream2(virtual pin V2)
    else:
        sense.clear()

if __name__ == "__main__":
    print("Blynk application started. Listening for events...")

    try:
        while True:
            blynk.run()
            if is_shaken():
                print("Security device moved.")
                blynk.log_event("movement_event")
                sense.clear(RED)
                blynk.virtual_write(3, 1) #movement on V3
                time.sleep(2)
                sense.clear(GREEN)
                blynk.virtual_write(3, 0)

            blynk.virtual_write(0, round(sense.temperature, 2)) #update temp on V2
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("Blynk application stopped.")
        sense.clear()