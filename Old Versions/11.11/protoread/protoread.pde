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

    //Initiate Print
	Serial.begin(9600);
}
void loop()
{
	int V0=analogRead(S0);
	int V1=analogRead(S1);
	int V2=analogRead(S2);
	int V3=analogRead(S3);
	int V4=analogRead(S4);
	int V5=analogRead(S5);
	

//Math Functions that convert analogRead numbers to recognizable voltages
	float X0 = (V0/1024.0)*5.0;
	float X1 = (V1/1024.0)*5.0;
	float X2 = (V2/1024.0)*5.0;
	float X3 = (V3/1024.0)*5.0;
	float X4 = (V4/1024.0)*5.0;
	float X5 = (V5/1024.0)*5.0;
	
//------------------------------------------------------------Debugging------------------------------------------------------------
	//Print values read from sensors (voltages)
		Serial.print(digitalRead(S0));
		Serial.print(digitalRead(S1));
		Serial.print(digitalRead(S2));
		Serial.print(digitalRead(S3));
		Serial.print(digitalRead(S4));
                Serial.println(digitalRead(S5));
		delay(500);//prints sensor voltages twice a second
		
//------------------------------------------------------------Commands------------------------------------------------------------
	if (V1>675)
	{analogWrite(L,0);}
	else
	{analogWrite(L,127);}
	
	if (V2>675)
	{analogWrite(R,0);}
	else
	{analogWrite(R,127);}
}//end
