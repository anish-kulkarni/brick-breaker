//brickbreaker
#include<simplecpp>
#include<iostream>
using namespace std;
// global variables
    int m,n,N; //number of rows and columns of bricks and total number of bricks left
    double H=400,L=200, //height ,length of window
           x,y,vx,vy, //position and velocity components of ball
           d,dp,s,h,delta, //present position(distance from left wall),previous position, size of bat, height of bat from ground, change in d due to one keypress
           l,t, //dimensions of bricks length and thickness
           dt;  //small time interval
    XEvent batMove; //right left arrow key press event for bat movement


int main()
{
    // initializations:
    m=4;n=5;N=m*n;
    x=L/2;y=H/20;vx=5;vy=20;
    d=L/2;s=L/2;h=H/20; delta= s/10;
    l=L/n;t=H/(3*m);
    dt=0.1;
    //create bricks
    bool Brix[m][n];
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            Brix[i][j]=1;
    //display
    initCanvas();
    Circle ball(x,y,3);
    Rectangle Boundary(L/2,H/2,L,H),bat(d,h,s,2),Brick[m][n],samplebrick(L/2,5*H/6,l,t);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            samplebrick.moveTo(l/2 + j*l , H-(t/2 + i*t));
            Brick[i][j] = samplebrick;
        }
    }
    samplebrick.hide();
    getClick();
    //game starts
    while(y>=h-abs(vy)*dt)
    {
        x += vx*dt;
        y += vy*dt;
        dp=d;
        int X,Y;    //stores coordinates of brick on which the ball is. The brick will then be broken if not yet broken.
        X = (x/l);
        Y = (H-y)/t;
        if(Y<0) Y=0;
        if(X<0) X=0;
        if(X>n-1) X=n-1;
        //chech collision with brick
        if(Y<m)
        {
            if(Brix[Y][X])
            {
                //break the brick
                Brix[Y][X]=0;
                Brick[Y][X].hide();
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
        //collision with walls
        if(y>H)
            vy=-vy;
        if(x< vx*dt*0.5 && vx<0)
            vx=-vx;
        if(x>L-vx*dt*0.5 && vx>0)
            vx=-vx;
        // change position of bat
        if(checkEvent(batMove) && keyPressEvent(batMove))
        {
            char c = charFromEvent(batMove);
            if(c=='a')
            {
                if(d-s/2 > 0)
                    d -= delta;
            }
            if(c=='d')
            {
                if(d+s/2 <L)
                    d += delta;
            }
        }
        //collision with bat
        if(y<h+abs(vy)*dt*0.5 && y>h-abs(vy)*dt*0.5 && x>d-s/2 && x<d+s/2 &&vy<0)
        {
            //vx += dp-d;
            vy=-vy;
        }
        ball.moveTo(x,y);
        bat.moveTo(d,h);
    }
}
