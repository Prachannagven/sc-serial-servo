# ⚙️ The Servo Motors from Hell  
_(This is not slander. Just raw truth.)_

The SC and ST series serial servos are **powerful**, **high-torque**, and **painfully finicky** to control. They're fantastic hardware-wise — but their software setup is something else. Out of the box, they come configured to operate at a painful **1,000,000 baud rate**. Sure, most microcontrollers *can* handle that. But at that speed, **timing becomes a serious issue**, especially if you (like me) started with using an Arduion Uno with the `SoftwareSerial.h` library to communicate.

You can't just grab an Arduino Uno and start debugging like it's a weekend project — oh no. You need to **first drop the baud rate** and **then** control them using your microcontroller like a sane person.

---

## 🗂️ Repo Structure

This repository has two major sections:

- **Python scripts** – Used for configuration and setup.
- **Arduino code** – (Mostly) bare-metal control code for the servo.  
  Uses `SoftwareSerial` because the Uno only has one hardware UART.

> 📚 Refer to [Waveshare's official page](https://www.waveshare.com/wiki/Bus_Servo_Adapter_(A)) for complete technical documentation. But I'll be honest — as a beginner, and even as an ameteur, it isn't the easiest read. That's why I've added more heavily commented and readable C and Python files.

---

## 🛠️ Getting Started

Before doing anything, **install the Python library** provided on Waveshare’s wiki. Once you download the zip folder, go to terminal and navigate there before running the `activate.bat` file in the `stservo-env\Scripts\` directory. Once you're in the "stservo-env" environment, you can navigate to the appropriate folder and run `python -m pip install -r requirements.txt`. 

Once everything has been installed, you can move to the `STServo_Python/STServo_Python/stservo-env/myscripts` directory and open the script named:  
**`change_baud_rate.py`**

### 🔧 Configuring the Script

Scroll down to the `"settings"` section and edit the following:

- `DEVICENAME`: Set this to the COM port your USB adapter is connected to. You can find the COM port using the device manager. Use the same format as the example.
- `OLD_BAUDRATE`: Set this to `1000000` (the factory default).
- `NEW_BAUDRATE_CODE`: This is a magic register value.  
  For example, `0x06` corresponds to `57600` baud. Much more manageable! For other baud rates, consult the memory tables.

Optional:
- `SERVO_ID`: If your servo ID isn't the default, update this too.

### 🏃‍♂️ Running It

Once you're done editing, you can save the file, and - ensuring you're still in the `(stservo-env)` environent - can run the following command (in the correct folder).

```bash
python change_baud_rate.py
```

It should say **"communication successful"**.  
> ⚠️ Don’t worry if it throws `"failed to set baud rate"` — that's a bug I'm still working on.

**(Edit:** The issue was that I powered the board with **12V** instead of the ideal **9V**, which caused the servo to misbehave. I found this by inspecting the error byte returned from the servo and tracing it back using the datasheet. Pain, but educational.)

---

## 🎯 First Script to Run

Once the baud rate is reduced, jump over to the `C/` folder and open the **ping example** in the Arduino IDE.

If the ping works — you're golden. From there, you can start issuing real commands.

---

## 🧠 Understanding the SC/ST Servos

These serial servos operate using an **internal bank of registers**. Every operation you want to perform — from rotation to telemetry — is just a matter of reading/writing to the right register.

> ✅ A full register map is available in the `additional documents/` folder.

### 🥶 Example: Reading the Temperature

Yes, these servos **have internal temperature sensors**. Reading the temperature is just as straightforward (or confusing) as sending a rotation command — just a different register.

### 🔐 Locking & Unlocking EEPROM

You’ll often want to make **persistent changes** (e.g., changing the servo ID). To do this:

1. Unlock the EEPROM.
2. Write your desired value.
3. Lock the EEPROM again.

This took me a while to get working, and I’ll keep updating the README with bugs I encounter and fixes that worked.

---

## 💡 Random but Useful Notes

- **Uno + 1 Mbps UART = 🔥**  
  Don’t try it unless you’re into pain. Lower the baud rate first.

- **SoftwareSerial has limits**  
  It technically supports up to 115200 baud, but pushing it higher is risky.

- **Debugging sucks at 1 Mbps**  
  Especially with a single UART. Trust me — change the baud rate.

---

Stay strong, servo warriors.  
May your packets never be corrupted and your IDs always respond.
