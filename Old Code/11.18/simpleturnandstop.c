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
	if (V1<425&&V2<425&&V0<425&&V3<425)
	{
		analogWrite(L,45);
		analogWrite(R,45);
	}
	if (V1<425&&V2<425&&V0<425&&V3>425)
	{
		analogWrite(L,45);
		analogWrite(R,0);
	}
	if (V1<425&&V2<425&&V0>425&&V3<425)
	{
		analogWrite(L,0);
		analogWrite(R,45);
	}
	if (V1>425&&V2>425)
	{
		if (V0>425&&V3>425&&V4>425&&V5>425)
		{
			analogWrite(L,0);
			analogWrite(R,0);
		}
		if (V0>425&&V3>425&&(V4<425||V5<425))
		{
			analogWrite(L,45);
			analogWrite(R,45);
		}
		if (V0<425&&V3<425&&((V4>425&&V4<675)||(V5>425&&V5<675)))
		{
			analogWrite(L,0);
			analogWrite(R,45);
		}
		if (V0<425&&V3<425&&!((V4>425&&V4<675)||(V5>425&&V5<675)))
		{
			analogWrite(L,45);
			analogWrite(R,0);
		}
	}
	if (V1<425&&V2>425)
	{
		analogWrite(L,45);
		analogWrite(R,0);
	}
	if (V1>425&&V2<425)
	{
		analogWrite(L,0);
		analogWrite(R,45);
	}
}
