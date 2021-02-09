# ps5controllermouse
With this program running you can use the playstation 5 controller (DualSense) as a mouse. In addition it provides basic keyboard input (shift and control key).
For the controller communication the program utilizes the [DualSense-Windows]() library. It is dynamically linked therefore you have to place the dll in the same directory as the executable.

For now the program does not have any GUI. It runs in a console and outputs the pressed buttons.
The button mapping is hardcoded and can not be changed.

It is likely that it will not work in games with anti-cheat because input is simulated by the SendInput function.


# Button mapping
- Left joystick: mouse movement
- Cross: left mouse click
- Square: right mouse click
- R2: mouse scroll wheel down
- L2: mouse scroll wheel up
- Triangle: Control key
- Circle: Shift key


# Supported platforms:
- Windows (Tested on windows 10. Should run on windows 7 and higher.)

# Bugs to fix
- Clicking on the virtual keyboard is not recognized.
