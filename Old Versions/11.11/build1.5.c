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
*/


//inputs
#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5

//outputs
#define L 5
#define R 6

void setup()
{	//(I/O pinModes)
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	
	//Initiate Print
	Serial.begin(9600);
}

void loop()
	{

	//Set initial value of LOW = Motor voltage at zero;
	digitalWrite(L, LOW);
	digitalWrite(R, LOW);
	
	//Math Functions that convert analogRead numbers to recognizable voltages
	float X0 = (analogRead(S0)/1024.0)*5.0;
	float X1 = (analogRead(S1)/1024.0)*5.0;
	float X2 = (analogRead(S2)/1024.0)*5.0;
	float X3 = (analogRead(S3)/1024.0)*5.0;
	float X4 = (analogRead(S4)/1024.0)*5.0;
	float X5 = (analogRead(S5)/1024.0)*5.0;
	
//------------------------------------------------------------Debugging------------------------------------------------------------
	//Print values read from sensors (voltages)
		if(X0 > 15 || X1 > 15 || X2 > 15 || X3 > 15 || X4 > 15 || X5 > 15)//checks if math is working properly
		{
		Serial.println("Math Error");
		}
			
		if(X0 < 15 && X1 < 15 && X2 < 15 && X3 < 15 && X4 < 15 && X5 < 15)//If math is done right, then print
		{
		Serial.println(X0);
		Serial.println(X1);
		Serial.println(X2);
		Serial.println(X3);
		Serial.println(X4);
		Serial.println(X5);
		Serial.println("--------");
		delay(750);//prints sensor voltages more than once a second
		}
		
//------------------------------------------------------------Commands------------------------------------------------------------


//SPLIT/STOP

		if (X0 > 1.5 && X3 > 1.5)//detect if split/stop with outer sensors detecting 1.5V threshold (grey)
		{
			//Stop
			if (X1 > 3.3 && X2 > 3.3 && X4 > 3.3 && X5 > 3.3)//detect stop: voltage threshold of 3.3
			{	
				delay(1000)
				digitalWrite(L, LOW);//sends a 0V voltage
				digitalWrite(R, LOW);//sends a 0V voltage
				Serial.print("STOPPED");
			}
			
			//Split
			else
			{
				//Grey (Right Turn)
				if (X4 > 1.5 && X4 < 3.3)//detect grey between 1.5 and 3.3 volts
				{
					analogWrite(L, 127);//sends a voltage of 2.5V to the motor
					digitalWrite(R, LOW);//sends a 0V voltage
					Serial.print("Split Grey - Right TURN");
				}
				//White (Left Turn)
				if (X4 > 3.3 || X5 > 3.3)//detect white at 3.3 volts
				{	
					analogWrite(R, 127);//sends a voltage of 2.5V to the motor
					digitalWrite(L, LOW);//sends a 0V voltage
					Serial.print("Split White - Left TURN");
				}
			}
		}
		
//TAPE AVOIDING

	//Going Straight
		if (X1 < 1.0 && X2 < 1.0)//Both sensors sense black
		{
			digitalWrite(L, HIGH);
			digitalWrite(R, HIGH);
		}

	//Turning
		//Left
		if(X1 < 3.3 && X2 > 3.3)//Right T.A. sensor senses white
		{
			digitalWrite(R, LOW);//sends a 0V voltage
			analogWrite(L, 127);//sends a voltage of 2.5V to the motor
		}
		//Right
		if(X1 > 3.3 && X2 < 3.3) //Left T.A. sensor senses white
		{
			analogWrite(R, 127);//sends a voltage of 2.5V to the motor
			digitalWrite(L, LOW);//sends a 0V voltage
		}
	}
}//end