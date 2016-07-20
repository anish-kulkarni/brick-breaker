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
           l,t,gap, //dimensions of bricks length and thickness. gap between display of bricks.
           dt,  //small time interval
           delay_time=5; //delay between showing successive frames

int main()
{
    // initializations:
    m=10;n=5;N=m*n;
    x=L/2;y=H/20;vx=10;vy=10;
    d=L/2;s=L/2;h=H/20; delta= s/20;
    l=L/n;t=H/(3*m); gap = 1;
    dt=0.1;
    //create bricks
    bool Brix[m][n];
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            Brix[i][j]=true;
    //display
    int q=DETECT,z;
    initgraph(&q,&z,"C:\\TC\\BGI");
    rectangle(0,0,L,H);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++){
            double xc = 0.5*l + j*l,
                   yc = H - (0.5*t + i*t);
            rectangle(xc - l/2 + gap, yc + t/2 - gap, xc + l/2 - gap, yc - t/2 + gap );
        }
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
        //check collision with brick
        int X,Y;    //stores coordinates of brick on which the ball is. The brick will then be broken if not yet broken.
        X = (x/l);
        Y = (H-y)/t;
        if(Y<0) Y=0;
        if(X<0) X=0;
        if(X>n-1) X=n-1;
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
                double xc = 0.5*l + X*l,
                       yc = H - (0.5*t + Y*t);
                setcolor(BLACK);
                rectangle(xc - l/2 + gap, yc + t/2 - gap, xc + l/2 - gap, yc - t/2 + gap );
            }
        }

        delay(delay_time);
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
