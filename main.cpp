#include<GL/glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXR 2.0
#define MIN_THETA 0.5233 //////0.261667  // 30 degree in radian
#define WIDTH 100.0
#define HEIGHT 100.0
#define MAXSPEED 0.8
#define MAXGOOD 20
#define MAXBAD 30

void ballupdate(int k);
time_t start;
int gameover;
int scrwidth,scrheight;
int score,running; // initialized with zero

class ball
{

  GLfloat cenx,ceny,radius;
  GLfloat r,g,b;
  float increment,velocity;
  int direction,type,active,delay,waiting;
  time_t start;


  public:
    ball();
    void init(int k=-1);
    void setPos(GLfloat x,GLfloat y);
    void setColor(GLfloat re,GLfloat gr,GLfloat bl);
    void setRadius(GLfloat r);
    void setIncrement(int inc);
    void setDirection(int dir);
    void setVelocity(int vel);
    void display();
    void update(int typ=-1);

};

ball::ball()
{
  type=-999; // type of ball not decided
  waiting=0;
  active=0;
  running=0;

}

void ball::init(int k)
{
  float prob;
  //activate
  if(active) return ;
  if(!waiting && !active)
  {
    prob=((rand()%100)/100.0);
    if(prob<0.5 && k!=100) // activation failed
    {
      active=0;
      start=time(NULL);
      delay=rand()%10; // random number of seconds to wait
      waiting=1;
      return ;
    }
    else active=1;
  }
  else // on waiting state
  {
    if(difftime(time(NULL),start)>=delay)
    {
      //end of wait
      waiting=0;
      active=1;
    }
    else
      return ; // continue waiting
  }
  //activation successful


  radius=((rand()%99)/100.0) * MAXR;
  increment=MIN_THETA;

  velocity=((rand()%99)/100.0)*MAXSPEED;
  prob=((rand()%100)/100.0);

  if(prob <0.25)
  {
    cenx=0;
    ceny=((rand()%100)/100.0)*HEIGHT;
    direction=0; // east
  }
  else if(prob <0.5)
  {
    ceny=0;
    cenx=((rand()%100)/100.0)*WIDTH;
    direction=1; // north
  }
  else if(prob <0.75)
  {
    ceny=HEIGHT;
    cenx=((rand()%100)/100.0)*WIDTH;
    direction=2; //south
  }
  else
  {
    ceny=((rand()%100)/100.0)*HEIGHT;
    cenx=WIDTH;
    direction=3;  //west
  }


  if(k==1 || type==1)
    setColor(0,1,0);
  else if(k==0 || type==0)
    setColor(1,0,0);
  if(type==-999) type=k;


}

void ball::setPos(GLfloat x,GLfloat y)
{
  if(x<=WIDTH && y<=HEIGHT && x>=0 && y>=0)
  {
    cenx=x;
    ceny=y;
  }
}

void ball::setColor(GLfloat rr,GLfloat gg,GLfloat bb)
{
  r=rr;
  g=gg;
  b=bb;
}

void ball::setRadius(GLfloat r)
{
  radius = r;
}

void ball::setIncrement(int inc)
{
  if(inc<360)
  increment=inc*2*3.14/360;
}

void ball::setDirection(int dir)
{
  if(dir>=0 && dir<=3)
    direction=dir;
}

void ball::setVelocity(int vel)
{
  if(vel>0 && vel <= MAXSPEED )
    velocity=vel;
}

void ball::display()
{
  if(!active) return ;

  float i;
  GLfloat x,y;
  glBegin(GL_POLYGON);
  glColor3f(r,g,b);
  //glutSolidSphere(radius,0,0);
  for(i=0;i<10;i+=increment)
  {

    x=radius*cos(i);
    y=radius*sin(i);
    glVertex2f((GLfloat)cenx+x,(GLfloat)ceny+y);
  }
  glEnd();
}

void ball::update(int typ)
{
if(active)
  switch(direction)
  {
    case 0:cenx+=velocity;//east
      if(cenx>WIDTH) {
	active=0;
	running=0;
	init(typ);
      }
      break;
    case 1:ceny+=velocity;//north
      if(ceny>HEIGHT) {
	active=0;
	running=0;
	init(typ);
      }
      break;
    case 2:ceny-=velocity;//south
      if(ceny<0) {
	running=0;
	active=0;
	init(typ);
      }
      break;
    case 3:cenx-=velocity;//west
      if(cenx<0) {
	active=0;
	running=0;
	init(typ);
      }
      break;
    default:printf("error\n");
  }

 else
 {
   this->init(typ);
 }
}

ball good[MAXGOOD],bad[MAXBAD],pawn;

void init()
{
  glClearColor(1,1,1,1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,WIDTH,0,HEIGHT);
  glMatrixMode(GL_MODELVIEW);
}

void gameinit()
{
    int i;
  for(i=0;i<MAXGOOD;i++)
  {
    good[i].init(1);  // 1 for good
  }
  for(i=0;i<MAXBAD;i++)
  {
    bad[i].init(0);  // 0 for bad
  }

  //pawn.init(100);
  //pawn.setRadius(0.25);
  //pawn.setPos(WIDTH/2,HEIGHT/2);
  //pawn.setIncrement(20) ; // hexagon
  //pawn.setColor(1,0.6,0.2);

}

void gameUpdate()
{
  int i;
  for(i=0;i<MAXGOOD;i++)
  {
    good[i].update(1);
  }
  for(i=0;i<MAXBAD;i++)
  {
    bad[i].update(0);
  }
  for(i=0;i<MAXGOOD;i++)
  {
    good[i].init(1);
  }
  for(i=0;i<MAXBAD;i++)
  {
    bad[i].init(0);
  }
}
void ballupdate(int k)
{
  if(gameover) return ;
  gameUpdate();
  glutPostRedisplay();
  glutTimerFunc(20,ballupdate,0);
}

void game()
{
  glClear(GL_COLOR_BUFFER_BIT);
  int i;
  for(i=0;i<MAXGOOD;i++)
  {
    good[i].display();
  }
  for(i=0;i<MAXBAD;i++)
  {
    bad[i].display();
  }
  //pawn.display();

  glFlush();
}

int main(int argc,char **argv)
{
  scrwidth=640;
  scrheight=480;
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
  glutInitWindowSize(scrwidth,scrheight);
  glutInitWindowPosition(50,50);
  glutCreateWindow("ClearBall");
  init();
  gameinit();
  glutDisplayFunc(game);
  glutTimerFunc(500,ballupdate,0);
  glutMainLoop();
}
