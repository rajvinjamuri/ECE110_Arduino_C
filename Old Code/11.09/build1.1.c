/* CHANGE LOG:

1.0 (November 8, 2011):
 	Straight movement, turns, and stop capability
	NO DELAYS INCLUDED; NO CODE FOR SPLIT ACTION; NO DEBUGGING CODE
1.1	(November 9, 2011):
	Initial values for outputs
	Math functions (not used in code)
	Changed 'int' to 'double' to allow for decimal numbers
	Sensor voltages printed twice a second ( for debugging)
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
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
	
	pinMode(L, OUTPUT);
	pinMode(R, OUTPUT);
}

void loop()
{//Main Functions

	//Set initial value of LOW = Motor voltage at zero; double used so PWM output is possible
	double L = LOW
	double R = LOW
	
	//Read Voltages (numbers stored are integers between 0 and 1023)
	int V0 = analogRead(S0);
	int V1 = analogRead(S1);
	int V2 = analogRead(S2);
	int V3 = analogRead(S3);
	int V4 = analogRead(S4);
	int V5 = analogRead(S5);
	
	//Math Functions that convert analogRead numbers to recognizable voltages
	double X0 = (V0/1024.0)*5.0;
	double X1 = (V1/1024.0)*5.0;
	double X2 = (V2/1024.0)*5.0;
	double X3 = (V3/1024.0)*5.0;
	double X4 = (V4/1024.0)*5.0;
	double X5 = (V5/1024.0)*5.0;
	
	//Print values read from sensors (voltages)
	for (int i=1;i>0;i++)//infinite loop
	{
		printf("%d",X0);
		printf("%d",X1);
		printf("%d",X2);
		printf("%d",X3);
		printf("%d",X4);
		printf("%d",X5);
		delay(500);//prints sensor voltages twice a second
	}
	
//------------------------------------------------------------Commands------------------------------------------------------------


//SPLIT/STOP

	if (V0 > 425 && V3 > 435)//detect if split/stop
		//425 is 2.0V threshold (grey)
	{
		//Stop
		if (V1 > 675 && V2 > 675 && V4 > 675 && V5 > 675)//detect stop
			//675 = voltage threshold of 3.3
		{	digitalWrite(L, LOW);//sends a 0V voltage
			digitalWrite(R, LOW);//sends a 0V voltage
		}
		
		//Split
		else
		{
			if (V4 < 675 || V5 < 675)//675 = voltage threshold of 3.3
			{	analogWrite(R, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(R, LOW);//sends a 0V voltage
			}
			else
			{	analogWrite(R, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(R, LOW);//sends a 0V voltage
			}
		}
	}
		
//TAPE AVOIDING
	else
	{
		//Left Motor Control	Note: 675 = voltage threshold of 3.3
		if (V1 > 675)	{digitalWrite(L, LOW);}//sends a 0V voltage
		else			{analogWrite(L, 127);}//sends a voltage of 2.5V to the motor
		
		//Right Motor Control	Note: 675 = voltage threshold of 3.3
		if (V2 > 675)	{digitalWrite(R, LOW);}//sends a 0V voltage
		else			{analogWrite(R, 127);}//sends a voltage of 2.5V to the motor
	}

}//end
	


	
	
/*	Extra tape avoiding code
	//Going Straight
		if (V1 < 675 && V2 < 675)
			//675 returns voltage of 3.3 threshold
		{
			digitalWrite(L, HIGH);
			digitalWrite(R, HIGH);
		}	
	//Turning Right
		else{
			if(V1 > 675 && V2 < 675)
				//675 returns voltage of 3.3 threshold
			{
				digitalWrite(L, LOW);//sends a 0V voltage
				analogWrite(R, 127);//sends a voltage of 2.5V to the motor
			}
	//Turning Left
			else 
			{
				analogWrite(L, 127);//sends a voltage of 2.5V to the motor
				digitalWrite(R, LOW);//sends a 0V voltage
			}
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
