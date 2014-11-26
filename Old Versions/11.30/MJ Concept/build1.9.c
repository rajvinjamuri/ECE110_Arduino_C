//------------------------------------------------------------Special Considerations---------------------------------------------
/*
	1. Make sure sensors/sensor bar/battery/ground/etc is all wired correctly with reliable connections.
	2. Verify that output voltages from sensors complies with 1.5 Volt "gray" threshold and 3.3 Volt "white" threshold.
*/
//------------------------------------------------------------Setup--------------------------------------------------------------

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
#define LED1 13//Split detection - ARDUINO BOARD LED
#define LED2 12//Color Detection - EXTERNAL LED
void setup()
{
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	pinMode(LED, OUTPUT);
}

void loop()
{
//Functions
	int sortcolor(int sensor)//Convert analogRead numbers to logical 0 (black), 1 (white), and 5 (grey).
	{
		int voltread=analogRead(sensor);
		if (voltread>675)//675 represents 3.3 volts
		{
			return 1;//white
		}
		else if (voltread<308)//308 represents 1.5 volts
		{
			return 0;//black
		}
		else
		{
			return 15;//grey
		}
	}
	void leftsplit(void)//Turns left at the white split
	{
		analogWrite(R, 45);
		analogWrite(L, 0);
		analogWrite(LED1,200); //Turn on Non color Split LED
		delay(400);//makes sure that the turn is complete
		analogWrite(R,45);
		analogWrite(L,45);
		delay(200);//get away from the split
		analogWrite(LED1,0); //Turn off Non color Split LED
	}
	void rightsplit(void)
	{
		analogWrite(R, 0);
		analogWrite(L, 45);
		analogWrite(LED2,200); //Turn on color Split LED
		delay(400);//makes sure that the turn is complete
		analogWrite(R,45);
		analogWrite(L,45);
		delay(200);//get away from the split
		analogWrite(LED,0); //Turn off color Split LED
	}
	void lefttapeavoid(void)
	{
		analogWrite(R, 45);
		analogWrite(L, 0);
		delay(100);
	}
	void righttapeavoid(void)
	{
		analogWrite(R, 0);
		analogWrite(L, 45);
		delay(100);	
	}
	void leftsharpturn(void)
	{
		analogWrite(R, 45);
		analogWrite(L, 0);
		delay(800);//makes sure that the turn is complete
	}
	void rightsharpturn(void)
	{
		analogWrite(R, 0);
		analogWrite(L, 45);
		delay(800);//makes sure that the turn is complete
	}
	void endstop(void)
	{
		analogWrite(R,0);
		analogWrite(L,0);
	}
	void moveforward(void)
	{
		analogWrite(R,45);
		analogWrite(L,45);
	}
// end Functions
// Assign logical values to each sensor (0,1,15 for black, white, and grey)
	int X0 = sortcolor(S0);
	int X1 = sortcolor(S1);
	int X2 = sortcolor(S2);
	int X3 = sortcolor(S3);
	int X4 = sortcolor(S4);
	int X5 = sortcolor(S5);
		
//------------------------------------------------------------Commands------------------------------------------------------------

//SPLIT/STOP

		//Stop
		if (X0+X1+X2+X3+X4+X5>3 && X0+X1+X2+X3+X4+X5<7)//more than three sensors see white and not a grey split
		{	
			endstop();
		}
		//Right Split
		if (X1+X2+X5+X6>4)//at least one sensor sees grey
		{
			rightsplit();
		}
		//Left Split
		if ((X1*X2)||(X1+X2+X5+X6<5))//two inner see white and no sensor sees grey
		{
			leftsplit();
		}
//TAPE AVOIDING SCHEME

		//Going Straight
		if (X0+X1+X2+X3==0)
		{
			moveforward();
		}
		//Tape avoid Right
		if (X1==0 && (X2+X3))//Include the outer sensors in the case that the car overshoots the track
		{
			righttapeavoid();
		}
		//Tape avoid Left
		if (X2 ==0 && (X0+X1))
		{
			lefttapeavoid();
		}
//Right Corner Turns
		//Sharp Right Turn
		if (X2*X3 && X0+X1==0)
		{
			rightsharpturn();
		}
		//Sharp Left Turn
		if (X2+X3==0 && X0*X1)
		{
			leftsharpturn();
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
1.9	Change float back to int, but 0,1, and 15 for logic operations (simplify loop coding)
		Turn all of the arduino commands into functions
		Add small delays to tape avoiding to increase effectiveness and reduce going off track
*/
