import serial

try:
    ser = serial.Serial('COM13', 76800, timeout=1)
    print("✅ Port opened at 76800 baud")
    ser.close()
except serial.SerialException as e:
    print("❌ Failed to open port at 76800 baud")
    print(e)
