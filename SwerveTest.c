#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_2,     TurnMotor,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     DriveMotor,    tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Project started July 7th 2014, project ended Augest 25th for mechanical issues.  Made by Andrew Slomski from FTC 7584

#include "JoystickDriver.c"     																																// Tells ROBOTC to include the driver file for the joystick.

int calculateDrivePower(int joyStickX, int joyStickY)																						// Function called in loop, converts x and y data on the joystick to the power data we need, and returns it
{																																																// Start of function
	if (joyStickX != 0 || joyStickY != 0)																													// Make sure that both the joystick values are not one (can't get the sqrt of 0, causes bad things)
	{																																															// Start of if statement
		int joyDist = (sqrt(joyStickX * joyStickX + joyStickY * joyStickY) / 181) * 100;						// Simplified version of the distance formula, because we know x1 and y1 will always be 0.  It then is divided by 181 (max distance) and multiplied by 100 (max motor power)
		return joyDist;																																							// Return the variable (rounded down, so it becomes an int)
	}																																															// End of one of the if conditions
	else																																													// If both the joystick values are 0 (this is redundant because the function shouldn't be called if neither of the joystick pos are below 10, but it will catch any errors)
	{																																															// Start of second if outcome
		return 0;																																										// Return 0: the motor won't turn
	}																																															// End of if statement
}																																																// End of function


int calculateTurnPower(int joyStickX, int joyStickY, int prevSlope)															// Function called in loop, converts x and y data on the joystick to the rotation data we need, and returns it
{																																																// Start of function
	if (((-joyStickY)/joyStickX) != prevSlope)																										// Make sure that the function isn't trying to return to the same position
	{																																															// Start of if statement
		int joySlope = ((-joyStickY)/(joyStickX));																									// Uses a simplified version slope formula to calculate the new slope of the joystick (see previous comment on simplified formulas)
		float slopeDifferance = (atan(abs(prevSlope - joySlope)));																	// Calculate the diffrence between the two slopes by using arctan and the absolute difference between the two slopes (rads)
		slopeDifferance = (slopeDifferance*180)/PI;																									// Convert from rads to degrees
		return floor(slopeDifferance);																															// Return the variable (rounded down, so it becomes an int)
	}																																															// End of if outcome
	else																																													// If the condition is fufilled
	{																																															// Start of else outcome
		return 0;																																										// Return 0, tell the motor not to turn
	}																																															// End of if statement
}																																																// End of function


task main()																																											// Main task
{																																																// Start of main task
	int previousJoySlope = 0;																																			// Initalize a varriable holding the previous slope of the joystick (by default, this is 0)
	int returnedPower = 0;																																				// A varriable to hold the number returned by the calculateTurnPower function, to prevent data from getting mucked up (ask me about why if you want)
	int TurnSpeed = 70;																																						// How fast you want the turn motor to turn when it's told to turn ( you can change this if you want, keep it within reason )
	while (true)																																									// Infinate loop to update joystick and preform functions
	{																																															// Start of inf loop
		getJoystickSettings(joystick);  																														// Update buttons and joysticks
		if (abs(joystick.joy1_x1) > 10 || abs(joystick.joy1_y1) > 10)																// When the joystick is at rest, it still reads values between 10 and -10 for both axises, we don't want that to muck up our data, so if both of them are beneath 10 (implying rest), don't try and preform any functions
		{																																														// Start of if statement
			if (joy1Btn(3) != 1)																																			// Check to see if the driver just wants to turn the wheels, without worrying about moving
			{																																													// Begining of if statement
				if (joystick.joy1_y1 > 0)																																// Because we're using slope to rotate the turn motor, two diffrent inputs can give us a slope of 1, therefor, we need to reverse the drive motor when the y is negetive, so we can determine between the slope
				{																																												// Start of if statement
					bMotorReflected[DriveMotor] = false; 																									// Make sure that the motor isn't reflected
				}																																												// End of if statement
				if (joystick.joy1_y1 < 0)																																// Because we're using slope to rotate the turn motor, two diffrent inputs can give us a slope of 1, therefor, we need to reverse the drive motor when the y is negetive, so we can determine between the slope
				{																																												// Start of if statement
					bMotorReflected[DriveMotor] = true; 																									// Make sure that the motor is reflected
				}																																												// End of if statement
				motor[DriveMotor] = calculateDrivePower(joystick.joy1_x1,joystick.joy1_y1); 						// Call the function to calculate the power to deliver to the drive motor, using the joystick values, then power the motor using the number it returns
			}																																													// End of if statement
			else																																											// If they just want to turn the wheels
			{																																													// Start of else statement
				motor[DriveMotor] = 0;																																	// Set the motor power to zero
			}																																													// End of else statement
			returnedPower = (calculateTurnPower(joystick.joy1_x1,joystick.joy1_y1,previousJoySlope));	// Use the function to set a varriable to the number the function returns
			if (returnedPower != 0)																																		// Check to make sure that the variable from the function isn't 0 (read the function for a list of possible reasons it mught return 0)
			{																																													// If it doesn't return 0
				nMotorEncoder[TurnMotor] = 0;																														// Reset the motor encoder from previous turns
				//while(nMotorEncoder[TurnMotor] < returnedPower)																				// While the motor hasn't reached the degrees it needs to turn
				//{																																											// Begining of while statement
					//motor[TurnMotor] = TurnSpeed;																												// Tell the motor to go
				//}																																											// End of while statment
				motor[TurnMotor] = 0;																																		// Tell the motor to stop
				previousJoySlope = ((-joystick.joy1_y1)/joystick.joy1_x1);															// Set the previous slope to the new slope for future runs
			}																																													// End of situation where it doesn't return 0
		}																																														// End of if statement
		else																																												// If both the joysticks have values below 10
		{																																														// Start of else actions
			motor[DriveMotor] = 0;																																		// If the joystick values are both below 10, the drive motor shouldn't be running
		}																																														// End of else actions
	}																																															// End of inf loop
}																																																// End of main task
