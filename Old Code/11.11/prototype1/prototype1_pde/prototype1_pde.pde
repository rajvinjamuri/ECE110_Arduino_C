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
	int V0=analogRead(S0);
	int V1=analogRead(S1);
	int V2=analogRead(S2);
	int V3=analogRead(S3);
	int V4=analogRead(S4);
	int V5=analogRead(S5);
	if ((V1>675&&V2>675)||(V1<675&&V2<675))
	{
		analogWrite(L,127);
		analogWrite(R,127);
	}
	if (V1<675&&V2>675)
	{
		analogWrite(R,0);
		analogWrite(L,127);
	}
	if (V1>675&&V2<675)
	{
		analogWrite(R,127);
		analogWrite(L,0);
	}
}


