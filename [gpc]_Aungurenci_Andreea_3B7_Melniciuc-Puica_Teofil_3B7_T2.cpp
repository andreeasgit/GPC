// Daca se doreste utilizarea bibliotecii GLUT trebuie
// inclus fisierul header GL/glut.h (acesta va include
// la GL/gl.h si GL/glu.h, fisierele header pentru
// utilizarea bibliotecii OpenGL). Functiile din biblioteca
// OpenGL sunt prefixate cu gl, cele din GLU cu glu si
// cele din GLUT cu glut.
#include "windows.h"
#include <iostream>
 #include <GL/glut.h>


#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//#include <GL/freeglut.h>

// dimensiunea ferestrei in pixeli
#define dim 300
using namespace std;
unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$

void Display1() {
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    double x,y,d;
    int k;
    for (double i=0.0; i<=19.0; i+=0.01)
    {
        k=int(i);
        x=double(i);
        if (i==0.0)
        {
            y=1;
        }
        else
        {
            d=fabs(i-k);

            if(d<0.5)
            {
                y=d/x;
                //std::cout<<d<<' '<<y<<' '<<x<<'\n';
            }
            else
                y=(1.0-d)/x;
                //std::cout<<d<<' '<<y<<'\n';
        }
        glVertex2f(x/20.0,y/1.1);
    }
    glEnd();
    /*
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1);
   double ratia = 0.05;
   double t;

   // calculul valorilor maxime/minime ptr. x si y
   // aceste valori vor fi folosite ulterior la scalare
   xmax = a - b - 1;
   xmin = a + b + 1;
   ymax = ymin = 0;
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP);
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP);
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
   */

}

// graficul functiei
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$,
void Display2() {
    double a = 0.3;
    double b = 0.2;
    double pi = 4 * atan(1);
    double x, y,t;
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for ( t = -pi; t < pi; t += 0.01)
    {
        x = 2*(a*cos(t)+b)*cos(t);
        y = 2*(a*cos(t)+b)*sin(t);
        glVertex2f(x/1.1,y/1.1);
    }
    glEnd();
    /*
   double pi = 4 * atan(1);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP);
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();*/
}

void Display3()
{
   double pi = 4 * atan(1);
   double xmax = 8 * pi;
   double a=0.2;
   int pas=2;
   double st_sus=0.99;
   double x1, y1, cx, cy;
   // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = -pi/2; t < pi/2; t += pi/300)
    {
      pas=(pas+1)%3;
      cx=x1;
      cy=y1;
      x1=a/(4*cos(t)*cos(t)-3);
      y1=(a*tan(t))/(4*cos(t)*cos(t)-3);
      if (x1<=0 && y1>=0 && x1>-st_sus && y1<st_sus)
      {

          if (pas==1 &&(cx-x1>0.02||cy-y1>0.01))
          {
              glBegin(GL_POLYGON);
              glVertex2f(x1,y1);
              glVertex2f(-st_sus,st_sus);
              glVertex2f(cx,cy);
              glEnd();
          }
          glVertex2f(x1,y1);
      }

    }
    glEnd();

    glColor3f(0.1,0.1,0.1); // negru
    glBegin(GL_LINE_LOOP);
    glVertex2f(-st_sus, st_sus);
    for (double t = -pi/2; t < pi/2; t += pi/300)
    {
      pas=(pas+1)%3;
      cx=x1;
      cy=y1;
      x1=a/(4*cos(t)*cos(t)-3);
      y1=(a*tan(t))/(4*cos(t)*cos(t)-3);
      if (x1<=0 && y1>=0 && x1>-st_sus && y1<st_sus)
      {
          glVertex2f(x1,y1);
      }
    }
    glEnd();
}
void Display4()
{
    double a=0.1;
    double b=0.2;
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t=-9.5; t<=9.5; t+=0.1)
    {
        double x1,y1;
        x1=a*t-b*sin(t);
        y1=a-b*cos(t);
        glVertex2f(x1,y1);
    }
    glEnd();
}

void Display5()
{
    double pi = 4 * atan(1);
    double R=0.1;
    double r=0.3;
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t=0.0; t<=2*pi; t+=0.05)
    {
        double x1,y1;
        x1=(R+r)*cos(r/R*t)-r*cos(t+r/R*t);
        y1=(R+r)*sin(r/R*t)-r*sin(t+r/R*t);
        glVertex2f(x1,y1);
    }
    glEnd();
}
void Display6()
{
    double pi = 4 * atan(1);
    double R=0.1;
    double r=0.3;
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t=0.0; t<=2*pi; t+=0.09)
    {
        double x1,y1;
        x1=(R-r)*cos(r/R*t)-r*cos(t-r/R*t);
        y1=(R-r)*sin(r/R*t)-r*sin(t-r/R*t);
        glVertex2f(x1,y1);
    }
    glEnd();
}
void Display7()//intrerupere la pi/4
{
    double pi = 4 * atan(1);
    double a=0.4;
    double r,t;
    double x,y;
    glColor3f(1,0.1,0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t=-pi/4.0; t<pi/4.0; t+=pi/200.0)
    {
        r=a*sqrt(2*cos(2*t));
        x=r*cos(t);
        y=r*sin(t);
        glVertex2f(x,y);
    }

    glEnd();

    glBegin(GL_LINE_STRIP);
    for (t=-pi/4.0; t<pi/4.0; t+=pi/200.0)
    {
        r=-a*sqrt(2*cos(2*t));
        x=r*cos(t);
        y=r*sin(t);
        glVertex2f(x,y);
    }
    glEnd();
}
void Display8()
{
    double a=0.02;
    double r,t;
    double x,y;
    double pi = 4 * atan(1);
    double e=exp(1.1);
    glColor3f(1,0.1,0.1); // rosu

    /*
    glBegin(GL_LINE_STRIP);
      glVertex2f(1.0,0);
      glVertex2f(-1.0,0);
    glEnd();
    */

    glBegin(GL_LINE_STRIP);
    //r=a*e^(1+t)
    for (t=0; t<=5.8*pi/6; t+=0.05)
    {
        //fabs(sin(x)) * exp(-sin(x))
        r=pow(e,1.0+t);
        x=r*cos(t);
        y=r*sin(t);
        glVertex2f(x/90.0,y/90.0);
    }
    glEnd();
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;
   case '2':
      Display2();
      break;
    case '3':
      Display3();
      break;
    case '4':
      Display4();
      break;
    case '5':
      Display5();
      break;
    case '6':
      Display6();
      break;
    case '7':
      Display7();
      break;
    case '8':
      Display8();
      break;
   default:
      break;
   }

   glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);

   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);

   glutKeyboardFunc(KeyboardFunc);

   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);

   glutMainLoop();

   return 0;
}
