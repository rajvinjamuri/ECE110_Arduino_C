//------------------------------------------------------------Special Considerations--------------------------------------------------------------
/*
	1. Make sure sensors/sensor bar/battery/ground/etc is all wired correctly with reliable connections.
	2. Verify that output voltages from sensors complies with 1.5 Volt "grey" threshold and 3.3 Volt "white" threshold.
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

int SPEED = 45;//the PWM value that is sent to motors -- via CA module -- when they are "on"

void setup()
{
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
}

//------------------------------------------------------------Car Movement Functions--------------------------------------------------------------

int sortColor(int sensor)//Convert analogRead numbers to logical 0 (black), 1 (white), and 15 (grey).
	{
		int voltRead = analogRead(sensor);
		
		if (voltRead>675){//675 represents 3.3 volts - white threshold
			return 1;//white
		}
			else if (voltRead<308){//308 represents 1.5 volts - grey threshold
				return 0;//black
			}
				else{
					return 15;//grey
				}
	}

void moveforward()
	{
		analogWrite(R,SPEED);
		analogWrite(L,SPEED);
	}
	
void leftTapeAvoid()
	{
		analogWrite(R, SPEED);
		analogWrite(L, 0);
		//delay(50);
	}

void rightTapeAvoid()
	{
		analogWrite(R, 0);
		analogWrite(L, SPEED);
		//delay(50);
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

void leftSharpTurn()
	{
		analogWrite(R, SPEED*1.2);
		analogWrite(L, 0);
		delay(20);//makes sure that the turn starts
		while(digitalRead(S2) == LOW){//makes sure the car is on track
			analogWrite(R, SPEED);
			digitalWrite(L, LOW);
		}
		analogWrite(L,SPEED);
		digitalWrite(R,LOW);
		delay(100);//stabilize car after sharp turn
		analogWrite(R,SPEED);
		analogWrite(L,SPEED);
	}

void rightSharpTurn()
	{
		analogWrite(R, 0);
		analogWrite(L, SPEED*1.2);
		delay(20);//makes sure that the turn starts
			while(digitalRead(S1) == LOW){
				analogWrite(L,SPEED);
				digitalWrite(R,LOW);
			}
		analogWrite(R,SPEED);
		digitalWrite(L,LOW);
		delay(100);//stabilize car after sharp turn
		analogWrite(R,SPEED);
		analogWrite(L,SPEED);
	}

void endStop()
	{
		analogWrite(R,0);
		analogWrite(L,0);
	}


// end Functions


//------------------------------------------------------------Commands--------------------------------------------------------------
void loop()
{
// Assign logical values to each sensor (0,1,15 for black, white, and grey)
	int X0 = sortColor(S0);
	int X1 = sortColor(S1);
	int X2 = sortColor(S2);
	int X3 = sortColor(S3);
	int X4 = sortColor(S4);
	int X5 = sortColor(S5);
		
////SPLIT/STOP
	int splitstop=X0+X1+X2+X3+X4+X5;//The logical sum of the sensors can be useful when testing for grey or stop conditions.
		//Right Split
		if (splitstop>6)//at least one sensor sees grey
		{
			rightSplit();
		}
		//Stop and Left Split
		else if(X1*X2)
		{
			if (splitstop>3)//more than three sensors see white and not a grey split means stop
			{	
				endStop();
			}
			else
			{
				leftSplit();//if not stop and not right split, and inside sensors are on means left split
			}
		}
////TAPE AVOIDING SCHEME

		//Straight
		if (X0+X1+X2+X3==0)
		{
			moveForward();
		}
		//Right
		if (X1==0 && (X2+X3))//Include the outer sensors in the case that the car overshoots the track
		{
			rightTapeAvoid();
		}
		//Left
		if (X2==0 && (X0+X1))
		{
			leftTapeAvoid();
		}
		
//Overshoot Track Turns
		//Sharp Right
		if (X3 && X1==0)
		{
			rightSharpTurn();
		}
		//Sharp Left
		if (X2==0 && X0)
		{
			leftSharpTurn();
		}
}//code end


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
1.9.1	Update language to standard notation (i.e righttapeavoid to rightTapeAvoid)
		Moved functions outside of loop.
		Fixed LED. Added LED1 and LED2 language that was needed. Normalized.
		Simplified output PWM with a "SPEED" integer.
		Fixed "X6" bug.
		Removed TA turn delays
1.9.5	Make Split Code loop directions more efficient
1.9.6	Make Sharp turn code simpler
*/
