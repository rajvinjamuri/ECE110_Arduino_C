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

int SPEED = 45;//the PWM value that is sent to motors -- via CA module -- when they are "on"
	
//Math Functions that convert analogRead numbers to recognizable voltages
float X0 = (analogRead(S0)/1024.0)*5.0;
float X1 = (analogRead(S1)/1024.0)*5.0;
float X2 = (analogRead(S2)/1024.0)*5.0;
float X3 = (analogRead(S3)/1024.0)*5.0;
float X4 = (analogRead(S4)/1024.0)*5.0;
float X5 = (analogRead(S5)/1024.0)*5.0;
	
void setup()
{
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
}

//------------------------------------------------------------Car Movement Functions--------------------------------------------------------------

void moveforward()
	{
		analogWrite(R,SPEED);
		analogWrite(L,SPEED);
	}
	
void leftTapeAvoid()
	{
		analogWrite(R, SPEED);
		analogWrite(L, 0);
		delay(100);
	}

void rightTapeAvoid()
	{
		analogWrite(R, 0);
		analogWrite(L, SPEED);
		delay(100);	
	}
	
//When car has overshot track, use outer sensors as tape avoiding sensors. 
//Turn is faster than other functions to ensure quick recovery
void leftSharpTurn()
	{
		analogWrite(R, SPEED*1.5);
		analogWrite(L, 0);
		delay(800);//makes sure that the turn is complete
	}

void rightSharpTurn()
	{
		analogWrite(R, 0);
		analogWrite(L, SPEED*1.5);
		delay(800);//makes sure that the turn is complete
	}

void endStop()
	{
		analogWrite(R,0);
		analogWrite(L,0);
	}

void leftSplit()//White Split - Left
	{
		analogWrite(R, SPEED);
		analogWrite(L, 0);
		digitalWrite(LED1, HIGH); //Turn on white Split LED
		delay(300);//makes sure that the turn is complete
		
		analogWrite(R,SPEED);//get away from the split
		analogWrite(L,SPEED);
		delay(200);
		digitalWrite(LED1, LOW); //Turn off white Split LED
	}

void rightSplit()//Grey Split - Right
	{
		analogWrite(R, 0);
		analogWrite(L, SPEED);
		digitalWrite(LED2, HIGH); //Turn on Grey Split LED
		delay(300);//makes sure that the turn is complete
		
		analogWrite(R,SPEED);//get away from the split
		analogWrite(L,SPEED);
		delay(200);
		digitalWrite(LED2, LOW); //Turn off Grey Split LED
	}
// end Functions


//------------------------------------------------------------Commands------------------------------------------------------------
void loop()
{

////SPLIT/STOP

		if (X1 > 1.5 && X2 > 1.5)//inside sensors non-black => Only split or stop scenarios
		{
			//Stop
			if (X0 > 3.3 && X3 > 3.3)//all front sensors see white
			{endStop();}
			
			else
			{
				//Grey = Right Turn (Arduino AND External LED Activated)
				if ((X4 > 1.5 && X4 < 3.3) || (X5 > 1.5 && X5 < 3.3))//either rear sensors sense grey
				{rightSplit();}
				
				//White = Left Turn (ONLY Arduino LED Activated)
				else
				{leftSplit();}
			}
		}
		
////TAPE AVOIDING SCHEME

//Going Straight
		if(X0 < 1.0 && X1 < 1.0 && X2 < 1.0 && X3 < 1.0)		{moveForward();}

//Turning
		//Left
		if(X1 < 3.3 && X2 > 3.3)
		{leftTapeAvoid();}
		
		//Right
		if(X1 > 3.3 && X2 < 3.3)
		{rightTapeAvoid();}
	
		//Left Sharp
		if(X0 > 3.3 && X1 > 3.3)
		{leftSharpTurn();}
		
		//Right Sharp
		if(X2 > 3.3 && X3 > 3.3)
		{rightSharpTurn();}
}//code end


/* CHANGE LOG:
1.0		Straight movement, turns, and stop capability
			NO DELAYS INCLUDED; NO CODE FOR SPLIT ACTION; NO DEBUGGING CODE
1.1		Initial values for outputs
			Math functions (not used in code)
			Changed 'int' to 'float' to allow for decimal numbers
			Sensor voltages printed twice a second ( for debugging)
1.2		Changed initial L/R values to be set by digitalWrite(L/R, LOW);
			Replaced tape avoiding logic with v.Raj from v.Mohammed
			Updated voltages with real voltage values (post-calculation)
			Initialze print
			Fix print to Serial.print
			Removed pinMode for analog pins
			Removed extra "learning" code from lecture
1.4		Add print for special cases
			Simplified printing
			Fixed correct turning scheme
1.5		Split fix attempt
			Split simplification (robust+)
			Added delay in stop function
			Math debugging check/print
1.6		Remove initial voltage outputs
			Remove print/debugging functions
			Remove extra delays
			Reduce PWM output to 45
			Change most digitalOut to analogOut
			Remove some comments for simplicity
1.6.5		Simplify split logic
			adjust stop delay
1.7		Emergency Turns
			More definate stop
			Add another grey turn
1.7.5		Combine grey split code
			Add comments - few
1.8		Fix split bug and make split more efficient
			remove stop delay and make stop refer to front row only
1.8.5		Add LED output for split and color detection
			Added delays to emergency turns and split turns to ensure complete turn
			Make emergency turn faster than other functions
			Add 'Special Considerations'
			Format mods for programming ease
1.8.5fn	Combine functions from 1.9 while keeping scenarios
			Simplified output PWM with a "SPEED" integer.
			Simplify emergency/right-angle turns scenario
*/