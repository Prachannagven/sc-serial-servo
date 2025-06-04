import sys
import os

if os.name == 'nt':
    import msvcrt
    def getch():
        return msvcrt.getch().decode()
        
else:
    import sys, tty, termios
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    def getch():
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

sys.path.append("..")
from STservo_sdk import *                 # Uses STServo SDK library

# Settings
DEVICENAME = 'COM13'      # Replace with your actual port
OLD_BAUDRATE = 57600    # Current servo baudrate
NEW_BAUDRATE_CODE = 7     # STS_38400
SERVO_ID = 1              # Target servo ID

# Initialize port and packet handler
portHandler = PortHandler(DEVICENAME)
packetHandler = sts(portHandler)

# Open port
if not portHandler.openPort():
    print("Failed to open port.")
    exit()

# Set to the current baud rate (must match the servo's)
if not portHandler.setBaudRate(OLD_BAUDRATE):
    print("Failed to set initial baudrate.")
    exit()

# Write new baud rate to EEPROM
result, error = packetHandler.write1ByteTxRx(SERVO_ID, STS_BAUD_RATE, NEW_BAUDRATE_CODE)

# Check result
if result == COMM_SUCCESS and error == 0:
    print(" Baud rate successfully changed to 38400.")
    print(" You must now reconnect using 38400 baud.")
else:
    print("Failed to change baud rate.")
    print("Result:", packetHandler.getTxRxResult(result))
    print("Error :", packetHandler.getRxPacketError(error))

# Close port
portHandler.closePort()
