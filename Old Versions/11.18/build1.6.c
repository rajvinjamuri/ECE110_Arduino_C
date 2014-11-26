/* CHANGE LOG:
1.0
		Straight movement, turns, and stop capability
		NO DELAYS INCLUDED; NO CODE FOR SPLIT ACTION; NO DEBUGGING CODE
1.1
		Initial values for outputs
		Math functions (not used in code)
		Changed 'int' to 'float' to allow for decimal numbers
		Sensor voltages printed twice a second ( for debugging)
1.2
		Changed initial L/R values to be set by digitalWrite(L/R, LOW);
		Replaced tape avoiding logic with v.Raj from v.Mohammed
		Updated voltages with real voltage values (post-calculation)
		Initialze print
		Fix print to Serial.print
		Removed pinMode for analog pins
		Removed extra "learning" code from lecture
1.4	
		Add print for special cases
		Simplified printing
		Fixed correct turning scheme
1.5
		Split fix attempt
		Split simplification (robust+)
		Added delay in stop function
		Math debugging check/print
1.6
		Remove initial voltage outputs
		Remove print functions
		Remove extra delays
		Reduce PWM output to 45
		Change most digitalOut to analogOut
		Remove some comments for simplicity
		
*/


#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5
#define L 5
#define R 6

void setup()
{
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
}

void loop()
{
	
	//Math Functions that convert analogRead numbers to recognizable voltages
	float X0 = (analogRead(S0)/1024.0)*5.0;
	float X1 = (analogRead(S1)/1024.0)*5.0;
	float X2 = (analogRead(S2)/1024.0)*5.0;
	float X3 = (analogRead(S3)/1024.0)*5.0;
	float X4 = (analogRead(S4)/1024.0)*5.0;
	float X5 = (analogRead(S5)/1024.0)*5.0;
	
		
//------------------------------------------------------------Commands------------------------------------------------------------

//SPLIT/STOP

		if (X0 > 1.5 && X3 > 1.5)
		{
			//Stop
			if (X1 > 3.3 && X2 > 3.3 && X4 > 3.3 && X5 > 3.3)
			{	
				delay(100);
				digitalWrite(L, LOW);
				digitalWrite(R, LOW);
			}
			
			//Split
			else
			{
				//Grey = Right Turn
				if (X4 > 1.5 && X4 < 3.3)
				{
					analogWrite(L, 45);
					analogWrite(R, 0);
				}
				//White = Left Turn
				if (X4 > 3.3 || X5 > 3.3)
				{	
					analogWrite(R, 45);
					analogWrite(L, 0);
				}
			}
		}
		
//TAPE AVOIDING

	//Going Straight
		if (X1 < 1.0 && X2 < 1.0)
		{
			analogWrite(R, 45);
			analogWrite(L, 45);
		}

	//Turning
		//Left
		if(X1 < 3.3 && X2 > 3.3)
		{
			analogWrite(R, 0);
			analogWrite(L, 45);
		}
		//Right
		if(X1 > 3.3 && X2 < 3.3)
		{
			analogWrite(R, 45);
			analogWrite(L, 0);
		}
}//end