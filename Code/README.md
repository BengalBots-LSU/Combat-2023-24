# Overall what the program needs
### Type of drives we're working on
- Tank
  - two joysticks where the left joystick control the left side of the bot and the right joystick controls the right side
  - only takes y-axis value range of the joysticks 
- Arcade
  - two joysticks where the left joystick controls the forward motion while the right joystick determines the turning direction
  - left joystick takes the y-axis range while the right joystick takes the x-axis range 
### Constantly changing variables
- int leftJoystick
- int rightJoystick
- bool killSwitch
- bool activateWeapon
### Button Controls
- killSwitch -> sends a comman to the bot to stop moving instantly, communication should still be established between driver and bot
- activateWeapon -> sends a command for the bot to move the weapon
  -  Note: need to know final decision on weapon movement so we can program it to be continuously moving or just one movement

### Modules that are being used on the Arduino Mega 
- nRF24L01 -> bluetooth module
- joysticks x 2 -> controllers
- motor controllers: 
  - Sabertooth -> drive train
  - Talon SRX -> weapon controller

### Notes for motor controller
- We'll be using servo libraries
- When the bot turns on make sure the motorcontroller is set to the midpoint value to "calibrate"
