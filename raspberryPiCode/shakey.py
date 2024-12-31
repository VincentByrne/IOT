from sense_hat import SenseHat
import time

GREEN = (0, 255, 0)  # RGB for green
RED = (255, 0, 0)    # RGB for red

# Initialise Sense HAT
sense = SenseHat()
sense.clear(GREEN)  # Start with green color

# Define threshold for shake detection
SHAKE_THRESHOLD = 1.5  # Adjust as needed for sensitivity

def is_shaken():
    # Get raw accelerometer data
    shaken=False
    accel = sense.get_accelerometer_raw()
    x = abs(accel['x'])
    y = abs(accel['y'])
    z = abs(accel['z'])
    
    if x > SHAKE_THRESHOLD or y > SHAKE_THRESHOLD or z > SHAKE_THRESHOLD:
        shaken=True
    # Check if the acceleration exceeds the shake threshold on any axis
    return shaken

if __name__ == "__main__":
    while True:
        if is_shaken():
            print("Shakey shakey")
            sense.clear(RED)  # Set LEDs to red
            time.sleep(1)  # Keep red for a second
            sense.clear(GREEN)  # Reset back to green

        # Short delay to prevent high CPU usage
        time.sleep(0.1)
