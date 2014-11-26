/* CHANGE LOG:

1.0 (November 8, 2011):
 	Straight movement, turns, and stop capability
	NO DELAYS INCLUDED; NO CODE FOR SPLIT ACTION; NO DEBUGGING CODE
1.1	(November 9, 2011):
	Initial values for outputs
	Math functions (not used in code)
	Changed 'int' to 'float' to allow for decimal numbers
	Sensor voltages printed twice a second ( for debugging)
1.2	Changed initial L/R values to be set by digitalWrite(L/R, LOW);
	Replaced tape avoiding logic with v.Raj from v.Mohammed
	Updated voltages with real voltage values (post-calculation)
	Initialze print
	Fix print to Serial.print
	Removed pinMode for analog pins
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
{//(I/O pinModes)
	/*
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
	*/
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
	
	//Initiate Print
	Serial.begin(9600);
}

void loop()
{//Main Functions

	//Set initial value of LOW = Motor voltage at zero;
	digitalWrite(L, LOW);
	digitalWrite(R, LOW);
	
	//Read Voltages (numbers stored are integers between 0 and 1023)
	int V0 = analogRead(S0);
	int V1 = analogRead(S1);
	int V2 = analogRead(S2);
	int V3 = analogRead(S3);
	int V4 = analogRead(S4);
	int V5 = analogRead(S5);
	
	//Math Functions that convert analogRead numbers to recognizable voltages
	float X0 = (V0/1024.0)*5.0;
	float X1 = (V1/1024.0)*5.0;
	float X2 = (V2/1024.0)*5.0;
	float X3 = (V3/1024.0)*5.0;
	float X4 = (V4/1024.0)*5.0;
	float X5 = (V5/1024.0)*5.0;
	
	//Print values read from sensors (voltages)
	for (int i=1;i>0;i++)//infinite loop
	{
		Serial.print(X0);
		Serial.print(X1);
		Serial.print(X2);
		Serial.print(X3);
		Serial.print(X4);
		Serial.println(X5);
		delay(500);//prints sensor voltages twice a second
	}
	
//------------------------------------------------------------Commands------------------------------------------------------------


//SPLIT/STOP

	if (X0 > 1.5 && X3 > 1.5)//detect if split/stop with outer sensors detecting 1.5V threshold (grey)
	{
		//Stop
		if (X1 > 3.3 && X2 > 3.3 && X4 > 3.3 && X5 > 3.3)//detect stop: voltage threshold of 3.3
		{	digitalWrite(L, LOW);//sends a 0V voltage
			digitalWrite(R, LOW);//sends a 0V voltage
		}
		
		//Split
		else
		{
			if (X4 < 1.5 || X5 < 1.5)//detect grey at 1.5 volts
			{	analogWrite(R, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(L, LOW);//sends a 0V voltage
			}
			else
			{	analogWrite(L, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(R, LOW);//sends a 0V voltage
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
	//Turning Right
		else{
			if(X1 > 3.3 && X2 < 3.3)
				//675 returns voltage of 3.3 threshold
			{
				digitalWrite(R, LOW);//sends a 0V voltage
				analogWrite(L, 127);//sends a voltage of 2.5V to the motor
			}
	//Turning Left
			else 
			{
				analogWrite(R, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(L, LOW);//sends a 0V voltage
			}
			}
	
}//end
	


	
	
/*	Extra tape avoiding code
	else
	{
		//Left Motor Control	Note: 675 = voltage threshold of 3.3
		if (V1 > 675)	{digitalWrite(L, LOW);}//sends a 0V voltage
		else			{analogWrite(L, 127);}//sends a voltage of 2.5V to the motor
		
		//Right Motor Control	Note: 675 = voltage threshold of 3.3
		if (V2 > 675)	{digitalWrite(R, LOW);}//sends a 0V voltage
		else			{analogWrite(R, 127);}//sends a voltage of 2.5V to the motor
	}
*/
/*	Commands
char response = 'y';
char response2='n';

if (response == 'y'){
	blah blah
}

for (int i=0;i<10;i++){
	printf("%d",i);
}

for (int i=10;i>0;i++){
	printf("%d", i);
}


int add(int a, intb){
	int c;
	c=a+b;
	return c;
	
}

int y = add(2,3);
Int y = add(2.0,3); wont work
*/
/*	Blinking LED
#define LED 13

void setup()
{
  pinMode(13, OUTPUT);
}

void loop(){
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

*/
/*	Set LED
#define LED 13
#define BUTTON 7

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop(){
  int val = digitalRead(BUTTON);
  if (val==HIGH)
  {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  
}
*/
/*	Apple sleep LED


#define GLEN 9

void setup()
{
  pinMode(GLEN, OUTPUT);
}

void loop()
{
  int i=0;
  for (i=0;i<255;i++)
  {
    analogWrite(GLEN, i);
    delay(5);
  }
  for (i=254;i>0;i--)
  {
    analogWrite(GLEN, i);
    delay(5);

  }

}

*/
