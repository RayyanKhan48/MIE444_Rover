import serial
import time

# Initialize serial communication with the HM10
port = "COM9"  # Replace with the correct COM port
baudrate = 9600

ser = serial.Serial(port, baudrate, timeout=1)
time.sleep(1)  # Wait for connection to stabilize

print("Connected to HM10 on", port)

while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        # Split data into individual readings
        readings = data.split(",")
        # Assign each reading to a variable for clarity
        front = float(readings[0])
        left1 = float(readings[1])
        left2 = float(readings[2])
        right1 = float(readings[3])
        right2 = float(readings[4])
        rear = float(readings[5])
        bottom = float(readings[6])
        compass = (readings[7])
        angle = (readings[8])

       # Display in a structured box-like format
        print(f"""
      +-----------------------------+
      |        Front: {front} in        |
      |                             |
Left1: {left1} in                  Right1: {right1} in
      |                             |
      |        Bottom: {bottom} in       |
      |                             |
Left2: {left2} in                  Right2: {right2} in
      |                             |
      |        Rear: {rear} in         |
      +-----------------------------+
      Compass Reading: {compass}
        """)
        # Check conditions and print corresponding messages
        if front <= 3:
            print("STOP!")
        elif abs(left1 - left2) >= 3:
            print("STRAIGHTEN ROVER!")
        elif abs(right1 - right2) >= 3:
            print("STRAIGHTEN ROVER!")
        else:
            print("CLEAR, CAN PROCEED :)")

    time.sleep(1)  # Update every 3 seconds