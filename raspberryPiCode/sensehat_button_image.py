from sense_hat import SenseHat
from capture_image import capture_image

sense = SenseHat()
IMAGE_PATH="./images/sensehat_image.jpg"

def button_pressed(event):
    if event.action == "pressed":
       capture_image(IMAGE_PATH)
       print("Image captured using SenseHAT button!")

sense.stick.direction_middle = button_pressed
print("Press the button on the SenseHAT to capture an image.")
while True:
    pass  # Keep the script running to detect button presses
