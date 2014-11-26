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
	pinmode(A0, INPUT);
	pinmode(A1, INPUT);
	pinmode(A2, INPUT);
	pinmode(A3, INPUT);
	pinmode(A4, INPUT);
	pinmode(A5, INPUT);
	pinmode(L, OUTPUT);
	pinmode(R, OUTPUT);
	int rightturn=0;
	int leftturn=0;
	int bumps=0;
	int comparetoV;
	
	float L=0;
	float R=0;
}
void loop
{
	int V0=analogRead(S0);
	int V1=analogRead(S1);
	int V2=analogRead(S2);
	int V3=analogRead(S3);
	int V4=analogRead(S4);
	int V5=analogRead(S5);
	//test whether a turn has been activated
	if (rightturn==1||leftturn==1)
	{
		if (leftturn==1)//switch to left side tape avoiding for 10 changes in V2
		{
			if (V2<425)//tape seeking
			{
				analogWrite(L,0);
				analogWrite(R,127);
			}
			else
			{
				analogWrite(L,127);
				analogWrite(R,0);
			}
			if ((comparetoV<425 && V2>425)||(comparetoV>425 && V2<425))//marks a change in V2
			{
				bumps=bumps+1;
			}
			if (bumps>9)//this ends the left turn sequence to resume tape avoiding
			{
				leftturn=0;
				bumps=0;
			}
		}
		if (rightturn==1)//switch to right side tape avoiding for 10 changes in V1
		{
			if (V1<425)//tape seeking
			{
				analogWrite(L,127);
				analogWrite(R,0);
			}
			else
			{
				analogWrite(L,0);
				analogWrite(R,127);
			}
			if ((comparetoV<425 && V1>425)||(comparetoV>425 && V1<425))//marks a change in V1
			{
				bumps=bumps+1;
			}
			if (bumps>9)//this ends the left turn sequence to resume tape avoiding
			{
				rightturn=0;
				bumps=0;
			}
		}
	}
	else
	{
		//test for whether (split/stop) or tape avoiding
		if (V0>425 &&V3>425)  /*425 is lower limit of grey, while 675 is the upper limit*/
		{
			if(V1>675 &&V2>675 &&V4>675 &&V5>675) //stop
			{
			analogWrite(L,0);
			analogWrite(R,0);
			}
			else 
			{
				analogWrite(L,0);
				analogWrite(R,0);
				if (V4<675||V5<675)
				{
					rightturn=1;//turn right
					comparetoV=V1;
				}
				else
				{
					leftturn=1;//turn left
					comparetoV=V2;
				}
			}
		}
		else
		{
			if ((V0>675 && V3<675)||(V0<675 && V3>675))//tape avoiding for situation near a turn
			{
				if (V0>425)
				{
					analogWrite(L,0);
				}
				else
				{
					analogWrite(L,127);
				}
				if (V3>425)
				{
					analogWrite(R,0);
				}
				else
				{
					analogWrite(R,127);
				}
			}
			else //tape avoiding
			{
				if (V1>675&&V2>675)
				{
					analogWrite(L,127);
					analogWrite(R,127);
				}
				else
				{
					if (V1>675)
					{
						analogWrite(L,0);
					}
					else
					{
						analogWrite(L,127);
					}
					if (V2>675)
					{
						analogWrite(R,0);
					}
					else
					{
						analogWrite(R,127);
					}
				}
			}
		}
	}
}
