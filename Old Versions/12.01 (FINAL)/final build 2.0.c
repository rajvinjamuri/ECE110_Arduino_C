//------------------------------------------------------------Special Considerations-----------------------------------------------------------------
/*
	1. Make sure sensors/sensor bar/battery/ground/etc is all wired correctly with reliable connections.
	2. Verify that output voltages from sensors complies with 1.5 "gray" threshold and 3.3 "white" threshold.
*/
//------------------------------------------------------------Setup------------------------------------------------------------------------------------------

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
	
void setup(){
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
}

//------------------------------------------------------------Car Movement Functions--------------------------------------------------------------

void navigate(){

//TAPE AVOIDING NAVIGATION
		if(X1 < 3.3)	{analogWrite(L, SPEED);}//Left Motor Control
			else			{analogWrite(L, 0);}
			
		if(X2 < 3.3)	{analogWrite(R, SPEED);}//Right Motor Control
			else			{analogWrite(R, 0);}
		
//RIGHT ANGLE NAVIGATION
		//Left Sharp
		if(X0 > 3.3 && X1 > 3.3){
			analogWrite(R, SPEED);
			digitalWrite(L, LOW);
			delay(300);//makes sure that the turn starts
			while(digitalRead(S2) == LOW){
				analogWrite(R, SPEED);
				digitalWrite(L, LOW);
			}
			
		}
		////Right Sharp
		if(X2 > 3.3 && X3 > 3.3){
			analogWrite(L, SPEED);
			digitalWrite(R, LOW);
			delay(300);//makes sure that the turn is complete
			while(digitalRead(S1) == LOW){
				analogWrite(L, SPEED);
				digitalWrite(R, LOW);
			}
			
		}
		
	}
	
void endStop(){
		digitalWrite(R, LOW);
		digitalWrite(L, LOW);
	}

void leftSplit(){//White Split - Left
		analogWrite(R, SPEED);
		digitalWrite(L, LOW);
		digitalWrite(LED1, HIGH); //Turn on white Split LED
		delay(300);//makes sure that the turn is complete
		digitalWrite(LED1, LOW); //Turn off white Split LED
	}

void rightSplit(){//Grey Split - Right
		digitalWrite(R, LOW);
		analogWrite(L, SPEED);
		digitalWrite(LED2, HIGH); //Turn on Grey Split LED
		delay(300);//makes sure that the turn is complete
		digitalWrite(LED2, LOW); //Turn off Grey Split LED
	}


//------------------------------------------------------------Commands----------------------------------------------------------------------------------
void loop(){

//SPLIT/STOP
		//Grey = Right Turn (Arduino AND External LED Activated)
		if ((X1 > 1.5 && X1 < 3.3) || (X2 > 1.5 && X2 < 3.3) || (X4 > 1.5 && X4 < 3.3) || (X5 > 1.5 && X5 < 3.3)){//Any sensor that would be over the split detects grey
			rightSplit();
		}
		else if (X1 > 1.5 && X2 > 1.5){//Both inside sensors detect white => Only split or stop scenarios
			//Stop
			if (X0 > 3.3 && X3 > 3.3){//all middle sensors see white
				endStop();
			}
			
			else{
				leftSplit();//If not stop and not right split, and inside sensors are on means left split

			}
		}
		
//TAPE AVOIDING SCHEME
		else{
			navigate();
		}
		
}
//code end


/*------------------------------------------------------------Change Log:-------------------------------------------------------------------------------
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
1.8.5fnb Changed TA scheme to more simple/robust scheme as given in lab instruction
			Initiated this new navigation scheme
			Reformatted and updated comments for ease of reading and to shorten length of code
1.8.6		Cleaned and compiled code from v1.8.5bfunction
			Replaced any "analogWrite(X, 0)" with "digitalWrite(x, LOW)"
1.8.7		Made Split/Stop loop directions more efficient
			Added conditions to stop to avoid any accidental situations
2.0		Verified code and successfully compiled
*/
