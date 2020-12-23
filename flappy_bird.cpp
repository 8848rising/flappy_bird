#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 12, 4, 3);
int arr[100][2];
int cy=24;     //current y coordinate of the ball, initialised with 24th pixel
			   //x coordinate is always fixed, the ball does not move forward, the frame moves backward
float tt=0;    // this will store the last time of last button press (Jump)
float u=0;     // this is the upward vilocity the ball will gets on abutton press (Jump).
float a=18;    //this will store the accelaration for the ball (gravity)
int col=0;
int bt=0;      //this will store the time when the gameplay actually starts.
int fv=15;     //frame vilocity, initialised with 15 pixels per second
float fa=0.25; //frame acceleration
	
	void testdrawchar(void) 
	{
		//diplaying GAME OVER SCREEN with SCORE

	  	display.setTextSize(1.5);
	  	display.setTextColor(BLACK);
	  	display.setCursor(0,0);
	  	display.println();
	  	display.println("   Game over");
	  	display.println();
	  	display.println();
	  	display.print("   Score=");

	  	// the final score is basically the distance travelled by the ball forwarard of 
	  	// which is same as the distance travelled by the frame backward
	  	// which can be calculated by s=ut+(1/2)(a*t^2); u->frame vilocity & a->frame accelaration

	  	display.println((int)(((millis()-bt)*fv/1000)+((0.5*fa*(millis()-bt)*(millis()-bt))/1000000))/10);
  	}    
 	void cp(int d,int h,int g)
 	{
 		//create pillar function, responsible for drawing pillars and checking if the ball
 		//has colided into a pillar or not.
	   	float t=(float)(millis()-bt)/1000;
	   	int x=(int)(t*fv+0.5*fa*t*t);
	   	if(x-d>0&&x-d<84)
	  	{
	  		//if the pillar is in our frame then print it.
	    	display.fillRect(x-d, 0, 5, h, BLACK);
	    	display.fillRect(x-d, h+g, 5, 48-h-g, BLACK);
	    	if(74-x+d<=7&&x-d<=74 )
	    	{
	    		//if the boundaries of the ball touch the pillar then a colison has occoured.
	      		if(cy-3<=h||cy+3>=h+g)
	      		{
	        		analogWrite(5,128);
	        		delay(500);
	        		analogWrite(5,LOW);
	        		col++;
	      		}
	      		else
	      		{
	        		analogWrite(5,50);
	        		delay(10);
	        		analogWrite(5,LOW);
	      		}
	    	}
	  	}
	}
	void ball()
 	{
 		//function to print the ball
  		if(digitalRead(A2)==LOW)
  		{
    		delay(50);
    		tt=millis();
    		u=8;
    		a=18;
  		}
  		//newT is the time that has elapsed from the last button press(JUMP).
  		float newT=(float)((millis()-tt)/1000);
  		//current y of the ball is calculated by simply using s= ut + (1/2)at^2
  		cy=cy-(int)((u*newT)-(0.5*a*newT*newT));
  		display.fillCircle(84-10 , cy, 2, BLACK); 
 	}
	void setup() 
	{
  	// put your setup code here, to run once:
  		Serial.begin(9600);
  		display.begin();
  		display.setContrast(65);
  		pinMode(A2,INPUT);
  		pinMode(5,OUTPUT);
  		display.clearDisplay();
  		//creating random obstacles before hand
  		for(int i=0;i<100;i++)
  		{
    		arr[i][0]=random(5,35);
    		arr[i][1]=random(20,47-arr[i][0]); 
  		}
  		display.setTextSize(1);
   		display.setTextColor(BLACK);
  		display.setCursor(0,3);
  		//display.println("Ready When U R!!");
  
  		while(digitalRead(A2)==HIGH)
  		{
    		display.display();
    		delay(500);
    		display.println();
    		display.println("LONG PRESS TO ");
    		display.println();
    		display.println("     START");
    		display.display();
    		delay(500);
    		display.clearDisplay();
  		}
  		display.clearDisplay();

  		bt=millis(); // game play starts.
	}
	void loop() 
	{
  		// put your main code here, to run repeatedly:
  		if(cy-2<0||cy+2>48&&col==0)
  		{
  			//ball goes out of frame game ends
    		col++;
    		analogWrite(5,128);
    		delay(1000);
    		analogWrite(5,LOW);
   		}
		if(col==0)
		{ 
			// col==0 -> no colision yet
			//call create pillar for all the random pillars in our array.
			for(int a=0;a<100;a++)
			{
  				cp(a*40-(int)(0.5*.25*a*a),arr[a][0],arr[a][1]);
			}
			ball();
			//printing current score on top of the screen.
			display.setTextSize(2);
  			display.setTextColor(BLACK);
  			display.setCursor(0,3);
  			display.print("  ");
  			if((int)(((millis()-bt)*fv/1000)+((0.5*fa*(millis()-bt)*(millis()-bt))/1000000))/10<10)
  			{
    			display.print("00");
  			}
  			else if((int)(((millis()-bt)*fv/1000)+((0.5*fa*(millis()-bt)*(millis()-bt))/1000000))/10<100)
  			{
    			display.print("0");
  			}
  			display.println((int)(((millis()-bt)*fv/1000)+((0.5*fa*(millis()-bt)*(millis()-bt))/1000000))/10);
			display.display();
			display.clearDisplay();
		}
		else if(col==1)
		{
			//collision has occoured
  			testdrawchar();
  			display.display();
  			delay(1000000000);
		}
}
