//brickbreaker
#include<iostream>
#include<graphics.h>
#include<math.h>
using namespace std;
// global variables
    int m,n,N; //number of rows and columns of bricks and total number of bricks left
    double H=400,L=200, //height ,length of window
           x,y,vx,vy, //position and velocity components of ball
           d,dp,s,h,delta, //present position(distance from left wall),previous position, size of bat, height of bat from ground, change in d due to one keypress
           l,t, //dimensions of bricks length and thickness
           dt;  //small time interval


int main()
{
    // initializations:
    m=1;n=5;N=m*n;
    x=L/2;y=H/20;vx=5;vy=20;
    d=L/2;s=L/4;h=H/20; delta= s/20;
    l=L/n;t=H/(3*m);
    dt=0.02;
    //create bricks
    bool Brix[m][n];
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            Brix[i][j]=1;
    //display
    int d=DETECT,m;
    initgraph(&d,&m,"C:\\TC\\BGI");
    rectangle(0,0,L,H);
    //game starts
    while(y>=h-abs(vy)*dt)
    {
        //change position of ball
        x += vx*dt;
        y += vy*dt;
        dp=d;
        // change position of bat
        if(kbhit())
        {
            setcolor(BLACK);
            line(d-s/2,h,d+s/2,h);
            int c = 0;
            c = getch();
            if(c==75)
            {
                if(d-s/2 > 0)
                    d = max(0.0,d-delta);
            }
            if(c==77)
            {
                if(d+s/2 <L)
                    d = min(L,d+delta);
            }
        }
        setcolor(WHITE);
        circle(x,y,3);
        line(d-s/2,h,d+s/2,h);
        rectangle(0,0,L,H);
        int X,Y;    //stores coordinates of brick on which the ball is. The brick will then be broken if not yet broken.
        X = (x/l);
        Y = (H-y)/t;
        if(Y<0) Y=0;
        if(X<0) X=0;
        if(X>n-1) X=n-1;
        //check collision with brick
        if(Y<m)
        {
            if(Brix[Y][X])
            {
                //break the brick
                Brix[Y][X]=0;
                //bounce the ball
                int Xp,Yp; //coordinates of brick the ball was previously in
                Xp = ((x-vx*dt)/l);
                Yp = (H-(y-vy*dt))/t;
                if(X==Xp)       //bounced from above or below. so y component of velocity reversed
                        vy=-vy;
                if(Y==Yp)       //bounced from right or left. so x component of velocity reversed
                        vx=-vx;
                if(X!=Xp && Y!=Yp)  //bounced from corner. both components reversed.
                {
                    vx=-vx;vy=-vy;
                }
            }
        }

        delay(4);
        setcolor(BLACK);
        circle(x,y,3);
        //collision with walls
        setcolor(WHITE);
        if(y>H)
        {
            vy=-vy;
            rectangle(0,0,L,H);
        }
        if(x< vx*dt*0.5 && vx<0)
        {
            vx=-vx;
            rectangle(0,0,L,H);
        }
        if(x>L-vx*dt*0.5 && vx>0)
        {
            vx=-vx;
            rectangle(0,0,L,H);
        }
        //collision with bat
        if(y<h+abs(vy)*dt*0.5 && y>h-abs(vy)*dt*0.5 && x>d-s/2 && x<d+s/2 &&vy<0)
        {
            vy=-vy;
        }
    }
}
