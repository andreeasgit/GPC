#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "windows.h"
#include <GL/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 600

#define NRITER_JF 100
// modulul maxim pentru testarea apartenentei la mult.Julia-Fatou
#define MODMAX_JF 1000000
// ratii ptr. CJuliaFatou
#define RX_JF 0.01
#define RY_JF 0.01

unsigned char prevKey;
int nivel = 0;

class CComplex {
public:
  CComplex() : re(0.0), im(0.0) {}
  CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
  CComplex(const CComplex &c) : re(c.re), im(c.im) {}
	~CComplex() {}

  CComplex &operator=(const CComplex &c)
  {
    re = c.re;
    im = c.im;
    return *this;
  }

  double getRe() {return re;}
  void setRe(double re1) {re = re1;}

  double getIm() {return im;}
  void setIm(double im1) {im = im1;}

  double getModul() {return sqrt(re * re + im * im);}

  int operator==(CComplex &c1)
  {
    return ((re == c1.re) && (im == c1.im));
  }

  CComplex pow2()
  {
    CComplex rez;
    rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
    rez.im = 2.0 * re * im;
    return rez;
  }

	friend CComplex operator+(const CComplex &c1, const CComplex &c2);
	friend CComplex operator*(CComplex &c1, CComplex &c2);

	void print(FILE *f)
	{
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
  double re, im;
};

CComplex operator+(const CComplex &c1, const CComplex &c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator*(CComplex &c1, CComplex &c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
                c1.re * c2.im + c1.im * c2.re);
	return rez;
}

class CJuliaFatou {
public:
  CJuliaFatou()
  {
    // m.c se initializeaza implicit cu 0+0i

    m.nriter = 30;
    m.modmax = 100;
  }

  CJuliaFatou(CComplex &c)
  {
    m.c = c;
    m.nriter = 30;
    m.modmax = 100;
  }

  ~CJuliaFatou() {}

  void setmodmax(double v) {assert(v <= MODMAX_JF); m.modmax = v;}
  double getmodmax() {return m.modmax;}

  void setnriter(int v) {assert(v <= NRITER_JF); m.nriter = v;}
  int getnriter() {return m.nriter;}

  // testeaza daca x apartine multimii Julia-Fatou Jc
  // returneaza 0 daca apartine, -1 daca converge finit, +1 daca converge infinit
  int isIn(CComplex &x)
  {
    int rez = 0;
    // tablou in care vor fi memorate valorile procesului iterativ z_n+1 = z_n * z_n + c
    CComplex z0,z1;

    z0 = x;
    for (int i = 1; i < m.nriter; i++)
    {
      z1 = z0 * z0 + m.c;
      if (z1 == z0)
      {
        // x nu apartine m.J-F deoarece procesul iterativ converge finit
        rez = -1;
        break;
      }
      else if (z1.getModul() > m.modmax)
      {
        // x nu apartine m.J-F deoarece procesul iterativ converge la infinit
        rez = 1;
        break;
      }
      z0 = z1;
    }

    return rez;
  }

  // afisarea multimii J-F care intersecteaza multimea argument
  void display(double xmin, double ymin, double xmax, double ymax)
  {
    glPushMatrix();
    glLoadIdentity();

//    glTranslated((xmin + xmax) * 1.0 / (xmin - xmax), (ymin + ymax)  * 1.0 / (ymin - ymax), 0);
//    glScaled(1.0 / (xmax - xmin), 1.0 / (ymax - ymin), 1);
    // afisarea propriu-zisa
    glBegin(GL_POINTS);
    for(double x = xmin; x <= xmax; x+=RX_JF)
      for(double y = ymin; y <= ymax; y+=RY_JF)
      {
        CComplex z(x, y);
        int r = isIn(z);
//        z.print(stdout);
        if (r == 0)
        {
//          fprintf(stdout, "   \n");
          glVertex3d(x,y,0);
        }
        else if (r == -1)
        {
//          fprintf(stdout, "   converge finit\n");
        }
        else if (r == 1)
        {
//          fprintf(stdout, "   converge infinit\n");
        }
      }
    fprintf(stdout, "STOP\n");
    glEnd();

    glPopMatrix();
  }

private:
  struct SDate {
    CComplex c;
    // nr. de iteratii
    int nriter;
    // modulul maxim
    double modmax;
  } m;
};

class CMandelbrot {
public:
  CMandelbrot()
  {
    // m.c se initializeaza implicit cu 0+0i

    m.nriter = NRITER_JF;
    m.modmax = MODMAX_JF;
  }

  CMandelbrot(CComplex &c)
  {
    m.c = c;
    m.nriter = NRITER_JF;
    m.modmax = MODMAX_JF;
  }

  ~CMandelbrot() {}

  void setmodmax(double v) {assert(v <= MODMAX_JF); m.modmax = v;}
  double getmodmax() {return m.modmax;}

  void setnriter(int v) {assert(v <= NRITER_JF); m.nriter = v;}
  int getnriter() {return m.nriter;}

  // testeaza daca x apartine multimii Julia-Fatou Jc
  // returneaza 0 daca apartine, -1 daca converge finit, +1 daca converge infinit
  int isIn(CComplex &x)
  {
    int rez = 0;
    // tablou in care vor fi memorate valorile procesului iterativ z_n+1 = z_n * z_n + c
    CComplex z0,z1;

    for (int i = 1; i < m.nriter; i++)
    {
      z1 = z0 * z0 + x;
      if (z1 == z0)
      {
        // x nu apartine m.J-F deoarece procesul iterativ converge finit
        rez = 1;
        break;
      }
      else if (z1.getModul() > 2)
      {
        // x nu apartine m.J-F deoarece procesul iterativ converge la infinit
        //fprintf(stdout, "%f   \n", (z1.getModul()-2.0)*100);
        glColor3f(z1.getModul()-2.0,1.0-z1.getModul()+2.0, z1.getModul()-2.0);
        rez = -1;
        break;
      }
      z0 = z1;
    }

    return rez;
  }

  // afisarea multimii J-F care intersecteaza multimea argument
  void display(double xmin, double ymin, double xmax, double ymax)
  {
    glPushMatrix();
    glLoadIdentity();

//    glTranslated((xmin + xmax) * 1.0 / (xmin - xmax), (ymin + ymax)  * 1.0 / (ymin - ymax), 0);
    glScaled(2.0 / (xmax - xmin), 2.0 / (ymax - ymin), 1);
    // afisarea propriu-zisa
    glBegin(GL_POINTS);
    for(double x = xmin; x <= xmax; x+=RX_JF)
      for(double y = ymin; y <= ymax; y+=RY_JF)
      {
        CComplex z(x, y);
        int r = isIn(z);
//        z.print(stdout);
        if (r == 0)
        {
//          fprintf(stdout, "   \n");
          //glColor3f(1.0, 0.1, 0.1);
          //glVertex3d(x,y,0);
        }
        else if (r == -1){
//         fprintf(stdout, "   converge infinit\n");
          glVertex3d(x,y,0);
        }
        else if (r == 1)
        {
 //         fprintf(stdout, "   converge finit\n");
          //glColor3f(1.0, 0.1, 0.1);
          //glVertex3d(x,y,0);
        }
      }
    fprintf(stdout, "STOP\n");
    glEnd();

    glPopMatrix();
  }

private:
  struct SDate {
    CComplex c;
    // nr. de iteratii
    int nriter;
    // modulul maxim
    double modmax;
  } m;
};

class C2coord
{
public:
  C2coord()
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y)
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPunct : public C2coord
{
public:
  CPunct() : C2coord(0.0, 0.0)
  {}

  CPunct(double x, double y) : C2coord(x, y)
  {}

  CPunct &operator=(const CPunct &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void getxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  int operator==(CPunct &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void marcheaza()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)", m.x, m.y);
  }

};

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalizare();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalizare();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPunct getDest(CPunct &orig, double lungime)
  {
    double x, y;
    orig.getxy(x, y);
    CPunct p(x + m.x * lungime, y + m.y * lungime);
    return p;
  }

  void rotatie(double grade)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1)) * grade / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalizare();
  }

  void deseneaza(CPunct p, double lungime)
  {
    double x, y;
    p.getxy(x, y);
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * lungime, y + m.y * lungime);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalizare()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0)
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};

class CCurbaKoch
{
public:
  void segmentKoch(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0)
    {
      v.deseneaza(p, lungime);
    }
    else
    {
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p, v);
      p1 = v.getDest(p, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(-120);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v1(sqrt(3.0)/2.0, 0.5);
    CPunct p1(-1.0, 0.0);

    CVector v2(0.0, -1.0);
    CPunct p2(0.5, sqrt(3.0)/2.0);

    CVector v3(-sqrt(3.0)/2.0, 0.5);
    CPunct p3(0.5, -sqrt(3.0)/2.0);

    segmentKoch(lungime, nivel, p1, v1);
    segmentKoch(lungime, nivel, p2, v2);
    segmentKoch(lungime, nivel, p3, v3);
  }
};

class CArboreBinar
{
public:
  void arboreBinar(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0)
    {
      v.deseneaza(p, lungime);
    }
    else
    {
      arboreBinar(lungime, nivel - 1, p, v);
      p1 = v.getDest(p, lungime);

      v.rotatie(-45);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);

      v.rotatie(90);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 1.0);

    arboreBinar(lungime, nivel, p, v);
  }
};

class CArborePerron
{
public:
  void arborePerron(double lungime,
                    int nivel,
                    double factordiviziune,
                    CPunct p,
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0)
    {
    }
    else
    {
      v.rotatie(30);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(-90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(-30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, -1.0);

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(lungime, nivel, 0.4, p, v);
  }
};



class CCurbaHilbert
{
public:
  void curbaHilbert(double lungime, int nivel, CPunct &p, CVector &v, int d)
  {
    if (nivel == 0)
    {
    }
    else
    {
      v.rotatie(d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      v.rotatie(-d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.rotatie(-d * 90);
      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.rotatie(d * 90);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, 0.0);

    curbaHilbert(lungime, nivel, p, v, 1);
  }
};
class Patratel
{
public:
    double ct=0.05;

  void patratel(double xs, double ys, double xj, double yj, int lvl)
  {
      /*xs=xs-ct;
      ys=ys+ct;
      xj=xj-ct;
      yj=yj+ct;*/
      double xjum=(xj-xs)/2;
      double yjum=(yj-ys)/2;
      if (lvl==0)
      {
          //deseneaza dreptunghi
          glVertex2d(xs+ct, ys-ct);
          glVertex2d(xs+ct, yj+ct);
          glVertex2d(xj-ct, yj+ct);
          glVertex2d(xj-ct, ys-ct);
          //glRectd(xs-ct,ys-ct,xj+ct,yj+ct);
      }
      else
      {
          ct=ct/2;
          double xp3=(xj-xs)/3;
          double yp3=(yj-ys)/3;
          //deseneaza dreptunghi mijloc 5
          glVertex2d(xs+xp3, ys+yp3);
          glVertex2d(xs+xp3, ys+2*yp3);
          glVertex2d(xs+2*xp3, ys+2*yp3);
          glVertex2d(xs+2*xp3, ys+yp3);/*
          glVertex2d(xs+ct, ys-ct);
          glVertex2d(xs+ct, yj+ct);
          glVertex2d(xj-ct, yj+ct);
          glVertex2d(xj-ct, ys-ct);*/
          //glRectd(xs-ct,ys-ct,xj+ct,yj+ct);
          //patratel X8
          ct=ct*2;
          patratel(xs,ys,xs+xp3,ys+yp3,lvl-1);//1
          patratel(xs+xp3,ys,xs+2*xp3,ys+yp3,lvl-1);//2
          patratel(xs+2*xp3,ys,xs+3*xp3,ys+yp3,lvl-1);//3
          patratel(xs,ys+yp3,xs+xp3,ys+2*yp3,lvl-1);//4
          patratel(xs+2*xp3,ys+yp3,xs+3*xp3,ys+2*yp3,lvl-1);//6
          patratel(xs,ys+2*yp3,xs+xp3,ys+3*yp3,lvl-1);//7
          patratel(xs+xp3,ys+2*yp3,xs+2*xp3,ys+3*yp3,lvl-1);//8
          patratel(xs+2*xp3,ys+2*yp3,xs+3*xp3,ys+3*yp3,lvl-1);//9
      }

  }

  void afisare(double lungime, int nivel)
  {
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_QUADS);
    patratel(-1.0,1.0,1.0,-1.0,nivel);
    glEnd();
  }
};

class CArbore
{
public:
  void arbore(double lungime,
                    int nivel,
                    double factordiviziune,
                    CPunct p,
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0)
    {
    }
    else
    {
      v.rotatie(45);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(-90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
      p1 = p2;


      v.rotatie(110);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arbore(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1=p2;
      v.rotatie(-65);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);

      lungime /=2;
      p2 = p1;
      v.rotatie(40);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arbore(lungime*2 * factordiviziune, nivel - 1, factordiviziune, p1, v);
      p1 = p2;

      v.rotatie(-130);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arbore(lungime*2 * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 2.5);


    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arbore(lungime, nivel, 0.4, p, v);
  }
};
class Hexagon
{
public:
  void hexagon(double lungime, int nivel, CPunct &p, CVector v)
  {
        if (nivel%2==1)
        {
            //+60
            v.rotatie(-60);
            semihexa(nivel,lungime,60,p,v);
        }
        else
        {
            semihexa(nivel,lungime,-60,p,v);
        }
  }
  void semihexa(int lvl, double lungime, int unghi, CPunct &p, CVector &v)
  {
      CPunct p1;
      if (lvl==0)
      {

          v.deseneaza(p, lungime);
          p=v.getDest(p, lungime);
      }
      else
      {
          //p = v.getDest(p, lungime/2);
          semihexa(lvl-1,lungime/2,-unghi, p, v);
          v.rotatie(unghi);
          //p = v.getDest(p, lungime/2);
          semihexa(lvl-1,lungime/2, unghi, p, v);
          v.rotatie(unghi);
          //p = v.getDest(p, lungime/2);
          semihexa(lvl-1,lungime/2,-unghi, p, v);
      }
  }
  void afisare(double lungime, int nivel)
  {
    CVector v1(0.0, -0.5);
    CPunct p1(-0.5, 0.5);

    hexagon(lungime, nivel, p1, v1);
  }
};
// afisare curba lui Koch "fulg de zapada"
void Display1() {
  CCurbaKoch cck;
  cck.afisare(sqrt(3.0), nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

  nivel++;
}

// afisare arbore binar
void Display2() {
  CArboreBinar cab;
  cab.afisare(1, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

  nivel++;
}

// afisare arborele lui Perron
void Display3() {
  CArborePerron cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}

// afisare curba lui Hilbert
void Display4() {
  CCurbaHilbert cch;
  cch.afisare(0.05, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

  nivel++;
}
void Display5() {
  Patratel patr;
  patr.afisare(0.05, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  nivel++;
}
void Display6() {
  CArbore cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');


  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}
void Display7() {
  Hexagon hex;
  hex.afisare(1.0, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'x');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');


  nivel++;

}
void Display8() {
  CComplex c(-0.012, 0.74);
  CJuliaFatou cjf(c);
  glColor3f(1.0, 0.1, 0.1);
  cjf.setnriter(30);
  cjf.display(-1, -1, 1, 1);
}
void Display9() {
  CComplex c(0.0, 0.0);
  CMandelbrot cjf;

  glColor3f(1.0, 0.1, 0.1);
  cjf.setnriter(30);
  cjf.display(-2.0, -2.0, 2.0, 2.0);
}
void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
  switch(prevKey)
  {
    case '0':
      glClear(GL_COLOR_BUFFER_BIT);
      nivel = 0;
      fprintf(stderr, "nivel = %d\n", nivel);
      break;
    case '1':
      glClear(GL_COLOR_BUFFER_BIT);
      Display1();
      break;
    case '2':
      glClear(GL_COLOR_BUFFER_BIT);
      Display2();
      break;
    case '3':
      glClear(GL_COLOR_BUFFER_BIT);
      Display3();
      break;
    case '4':
      glClear(GL_COLOR_BUFFER_BIT);
      Display4();
      break;
    case '5':
      glClear(GL_COLOR_BUFFER_BIT);
      Display5();
      break;
    case '6':
      glClear(GL_COLOR_BUFFER_BIT);
      Display6();
      break;
    case '7':
      glClear(GL_COLOR_BUFFER_BIT);
      Display7();
      break;
    case '8':
      glClear(GL_COLOR_BUFFER_BIT);
      Display8();
      break;
    case '9':
      glClear(GL_COLOR_BUFFER_BIT);
      Display9();
      break;
    default:
      break;
  }

  glFlush();
}

void Reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
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


