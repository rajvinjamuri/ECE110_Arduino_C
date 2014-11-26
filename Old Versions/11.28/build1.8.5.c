//------------------------------------------------------------Special Considerations---------------------------------------------
/*
	1. Make sure sensors/sensor bar/battery/ground/etc is all wired correctly with reliable connections.
	2. Verify that output voltages from sensors complies with 1.5 "gray" threshold and 3.3 "white" threshold.
*/
//------------------------------------------------------------Setup---------------------------------------------------------------------

//inputs
#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5
//outputs
#define L 5//Motor
#define R 6//Motor
#define LED1 13//Split detection - ARDUINO BOARD LED
#define LED2 12//Color Detection - EXTERNAL LED

void setup()
{
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
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

////SPLIT/STOP

		if (X1 > 1.5 && X2 > 1.5)//inside sensors non-black => Only split or stop scenarios
		{
			//Stop
			if (X0 > 3.3 && X3 > 3.3)//all front sensors see white
			{	
				digitalWrite(L, LOW);
				digitalWrite(R, LOW);
			}
			
			//Since we either had a stop or a split, and we did not stop, then we must be at a split
			else
			{
				//Grey = Right Turn (Arduino AND External LED Activated)
				if ((X4 > 1.5 && X4 < 3.3) || (X5 > 1.5 && X5 < 3.3))
				{
					digitalWrite(LED1, HIGH);//turn on LED
					digitalWrite(LED2, HIGH);
					analogWrite(L, 45);
					analogWrite(R, 0);
					delay(200);//ensure complete turn
					digitalWrite(LED1, LOW);//turn off LED
					digitalWrite(LED2, LOW);
				}
				//White = Left Turn (ONLY Arduino LED Activated)
				else
				{	
					digitalWrite(LED1, HIGH);
					analogWrite(R, 45);
					analogWrite(L, 0);
					delay(200);//ensure complete turn
					digitalWrite(LED1, LOW);
				}
			}
		}
		
////TAPE AVOIDING SCHEME

//Going Straight
		if (X0 < 1.0 && X1 < 1.0 && X2 < 1.0 && X3 < 1.0)
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
	
	//When car has overshot track, use outer sensors as tape avoiding sensors. 
	//Turn is faster than other functions to ensure quick recovery
		//Left
		if (X1 < 3.3 && X2 < 3.3 && X0 > 3.3 && X3 < 3.3)
		{
			analogWrite(L,0);
			analogWrite(R,55);
			delay(800);//ensure complete turn
		}
		//Right
		if (X1 < 3.3 && X2 < 3.3 && X0 < 3.3 && X3 > 3.3)
		{
			analogWrite(L,55);
			analogWrite(R,0);
			delay(800);//ensure complete turn
		}
}//end


/* CHANGE LOG:
1.0	Straight movement, turns, and stop capability
		NO DELAYS INCLUDED; NO CODE FOR SPLIT ACTION; NO DEBUGGING CODE
1.1	Initial values for outputs
		Math functions (not used in code)
		Changed 'int' to 'float' to allow for decimal numbers
		Sensor voltages printed twice a second ( for debugging)
1.2	Changed initial L/R values to be set by digitalWrite(L/R, LOW);
		Replaced tape avoiding logic with v.Raj from v.Mohammed
		Updated voltages with real voltage values (post-calculation)
		Initialze print
		Fix print to Serial.print
		Removed pinMode for analog pins
		Removed extra "learning" code from lecture
1.4	Add print for special cases
		Simplified printing
		Fixed correct turning scheme
1.5	Split fix attempt
		Split simplification (robust+)
		Added delay in stop function
		Math debugging check/print
1.6	Remove initial voltage outputs
		Remove print/debugging functions
		Remove extra delays
		Reduce PWM output to 45
		Change most digitalOut to analogOut
		Remove some comments for simplicity
1.6.5	Simplify split logic
		adjust stop delay
1.7	Emergency Turns
		More definate stop
		Add another grey turn
1.7.5	Combine grey split code
		Add comments - few
1.8	Fix split bug and make split more efficient
		remove stop delay and make stop refer to front row only
1.8.5	Add LED output for split and color detection
		Added delays to emergency turns and split turns to ensure complete turn
		Make emergency turn faster than other functions
		Add 'Special Considerations'
		Format mods for programming ease
*/