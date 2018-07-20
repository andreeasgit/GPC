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
int HEIGHT = 300;
int WIDTH = 300;

unsigned char prevKey;


class GrilaCarteziana
{
    int n;
    int m;
    bool M[100][100];
public:
    GrilaCarteziana()
    {
        n=8;
        m=8;
        clear_grid();
    }
    GrilaCarteziana(int x)
    {
        n=x;
        m=x;
        clear_grid();
    }
    GrilaCarteziana(int x, int y)
    {
        n=x;
        m=y;
        clear_grid();
    }
    void writePixel(int x, int y)
    {
        M[x][y]=M[x][y] xor 1;

        glPolygonMode(GL_FRONT,GL_FILL );
        glBegin(GL_POLYGON);

        float raza = 0.6/fmin(n,m);
        float pi = 3.1415926f;
        float ox;
        float oy;

        ox = (-0.8+(0.8/(0.8*(n/1.6)))*(x-1)); //(-1.0+2.0/n*(y-1) + (-1.0+2.0/n *y))/2.0;
        oy = (0.8-(0.8/(0.8*(n/1.6)))*(y-1));//(1.0-2.0/n*(x-1) + (1.0-(2.0/n * x)))/2.0;

        //std::cout<<"ox="<<ox<<" oy="<<oy<<"\n";
        for (float i = 0; i < 2*pi; i+=pi/6)
        {
            glVertex2f(ox+raza * cos(i), oy+ raza *sin(i));
        }

        glEnd();
    }

    void clear_grid()
    {
        for (int i=1; i<=n; i++)
            for (int j=1; j<=m; j++)
                M[i][j]=0;
    }

    void display_grid()
    {
        float pas_linie = 1.6/(float)n ;
        float pas_coloana = 1.6/(float)m ;

        glColor3d(0.5,0.5,0.5); // gri

        //glColor3d(1.0,1.0,1.0); //alb

        glPolygonMode(GL_FRONT,GL_LINE);
        glBegin(GL_LINES);
        for (float i=0.0; i<=pas_linie*(n+1);i+=pas_linie)
        {
            glVertex2f(-0.8,0.8-i);
            glVertex2f(0.8,0.8-i);
        }

        for (float j = 0.0; j<=pas_coloana*(m+1);j+=pas_coloana)
        {
            glVertex2f(-0.8+j,0.8);
            glVertex2f(-0.8+j,-0.8);
        }

        glEnd();
    }
    void segment(int x0, int y0, int xn, int yn)
    {
        float ox;
        float oy;
        glColor3d(1.0,0.1,0.1);
        glBegin(GL_LINE_STRIP);
        ox = (-0.8+(0.8/(0.8*(n/1.6)))*(x0-1)); //(-1.0+2.0/n*(y-1) + (-1.0+2.0/n *y))/2.0;
        oy = (0.8-(0.8/(0.8*(n/1.6)))*(y0-1));
        glVertex2f(ox,oy);
        ox = (-0.8+(0.8/(0.8*(n/1.6)))*(xn-1)); //(-1.0+2.0/n*(y-1) + (-1.0+2.0/n *y))/2.0;
        oy = (0.8-(0.8/(0.8*(n/1.6)))*(yn-1));
        glVertex2f(ox,oy);
        glEnd();
        glColor3d(0.5,0.5,0.5);
    }
    void afisare_segment_3(int x0, int y0, int xn, int yn)
    {
        segment(x0, y0, xn, yn);
        int dx=xn-x0;
        int dy=yn-y0;
        int d=2*dy-dx;
        int dE=2*dy;
        int dNE= 2*(dy-dx);
        int x=x0;
        int y=y0;
        //desenez
        writePixel(x,y);
        if (dx>dy)
        {
            while(x<xn)
            {
                if (d<=0)
                {
                    d+=dE;
                    x++;
                }
                else
                {
                    d+=dNE;
                    x++;
                    y++;
                }
                //desenez
                writePixel(x,y);
            }
        }
        else
        {
            d=2*dx-dy;
            while(y<yn)
            {
                 if (d<=0)
                {
                    d+=2*dx;

                    y++;
                }
                else
                {
                    d+=2*(dx-dy);
                    x++;
                    y++;
                }
                //desenez
                writePixel(x,y);
            }
        }

    }
};


void Display1() {
   GrilaCarteziana grila = GrilaCarteziana(15);
   grila.display_grid();
   grila.writePixel(1,1);
   grila.writePixel(2,6);
   grila.writePixel(6,2);
   grila.writePixel(13,11);
   grila.afisare_segment_3(1,1,15,6);

}


void Init(void) {
   // specifica culoarea unui buffer dupa ce acesta
   // a fost sters utilizand functia glClear. Ultimul
   // argument reprezinta transparenta (1 - opacitate
   // completa, 0 - transparenta totala)
   glClearColor(1.0,1.0,1.0,1.0);

   // grosimea liniilor
   glLineWidth(2);

   // dimensiunea punctelor
   glPointSize(4);

   // functia void glPolygonMode (GLenum face, GLenum mode)
   // controleaza modul de desenare al unui poligon
   // mode : GL_POINT (numai vf. primitivei) GL_LINE (numai
   //        muchiile) GL_FILL (poligonul plin)
   // face : tipul primitivei geometrice dpdv. al orientarii
   //        GL_FRONT - primitive orientate direct
   //        GL_BACK  - primitive orientate invers
   //        GL_FRONT_AND_BACK  - ambele tipuri
   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   printf("Call Display\n");

   // sterge buffer-ul indicat
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;

   default:
      break;
   }

   // forteaza redesenarea imaginii
   glFlush();
}

/*
   Parametrii w(latime) si h(inaltime) reprezinta noile
   dimensiuni ale ferestrei
*/

/*
   Parametrul key indica codul tastei iar x, y pozitia
   cursorului de mouse
*/
void KeyboardFunc(unsigned char key, int x, int y) {
   printf("Ati tastat <%c>. Mouse-ul este in pozitia %d, %d.\n",
            key, x, y);
   // tasta apasata va fi utilizata in Display ptr.
   // afisarea unor imagini
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void Reshape(int w, int h) {
   printf("Call Reshape : latime = %d, inaltime = %d\n", w, h);

   // functia void glViewport (GLint x, GLint y,
   //                          GLsizei width, GLsizei height)
   // defineste poarta de afisare : acea suprafata dreptunghiulara
   // din fereastra de afisare folosita pentru vizualizare.
   // x, y sunt coordonatele pct. din stg. jos iar
   // width si height sunt latimea si inaltimea in pixeli.
   // In cazul de mai jos poarta de afisare si fereastra coincid
   int wid = h * (float) WIDTH/HEIGHT;
   int hei = w * (float) HEIGHT/WIDTH;
   int left = w - wid;
   int up= h-hei;
    if (w>h)
        glViewport(0, 0, wid, h);
    else
        glViewport(0, 0, w, hei);


   //glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}
/*
   Codul butonului poate fi :
   GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
   Parametrul state indica starea: "apasat" GLUT_DOWN sau
   "eliberat" GLUT_UP
   Parametrii x, y : coordonatele cursorului de mouse
*/
void MouseFunc(int button, int state, int x, int y) {
   printf("Call MouseFunc : ati %s butonul %s in pozitia %d %d\n",
      (state == GLUT_DOWN) ? "apasat" : "eliberat",
      (button == GLUT_LEFT_BUTTON) ?
      "stang" :
      ((button == GLUT_RIGHT_BUTTON) ? "drept": "mijlociu"),
      x, y);
}

int main(int argc, char** argv) {
   // Initializarea bibliotecii GLUT. Argumentele argc
   // si argv sunt argumentele din linia de comanda si nu
   // trebuie modificate inainte de apelul functiei
   // void glutInit(int *argcp, char **argv)
   // Se recomanda ca apelul oricarei functii din biblioteca
   // GLUT sa se faca dupa apelul acestei functii.
   glutInit(&argc, argv);

   // Argumentele functiei
   // void glutInitWindowSize (int latime, int latime)
   // reprezinta latimea, respectiv inaltimea ferestrei
   // exprimate in pixeli. Valorile predefinite sunt 300, 300.
   glutInitWindowSize(WIDTH, HEIGHT);

   // Argumentele functiei
   // void glutInitWindowPosition (int x, int y)
   // reprezinta coordonatele varfului din stanga sus
   // al ferestrei, exprimate in pixeli.
   // Valorile predefinite sunt -1, -1.
   glutInitWindowPosition(100, 100);

   // Functia void glutInitDisplayMode (unsigned int mode)
   // seteaza modul initial de afisare. Acesta se obtine
   // printr-un SAU pe biti intre diverse masti de display
   // (constante ale bibliotecii GLUT) :
   // 1. GLUT_SINGLE : un singur buffer de imagine. Reprezinta
   //    optiunea implicita ptr. nr. de buffere de
   //    de imagine.
   // 2. GLUT_DOUBLE : 2 buffere de imagine.
   // 3. GLUT_RGB sau GLUT_RGBA : culorile vor fi afisate in
   //    modul RGB.
   // 4. GLUT_INDEX : modul indexat de selectare al culorii.
   // etc. (vezi specificatia bibliotecii GLUT)
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   // Functia int glutCreateWindow (char *name)
   // creeaza o fereastra cu denumirea data de argumentul
   // name si intoarce un identificator de fereastra.
   glutCreateWindow (argv[0]);

   Init();

   // Functii callback : functii definite in program si
   // inregistrate in sistem prin intermediul unor functii
   // GLUT. Ele sunt apelate de catre sistemul de operare
   // in functie de evenimentul aparut

   // Functia
   // void glutReshapeFunc (void (*Reshape)(int width, int height))
   // inregistreaza functia callback Reshape care este apelata
   // oridecate ori fereastra de afisare isi modifica forma.
   glutReshapeFunc(Reshape);

   // Functia
   // void glutKeyboardFunc (void (*KeyboardFunc)(unsigned char,int,int))
   // inregistreaza functia callback KeyboardFunc care este apelata
   // la actionarea unei taste.
   glutKeyboardFunc(KeyboardFunc);

   // Functia
   // void glutMouseFunc (void (*MouseFunc)(int,int,int,int))
   // inregistreaza functia callback MouseFunc care este apelata
   // la apasarea sau la eliberarea unui buton al mouse-ului.
   glutMouseFunc(MouseFunc);

   // Functia
   // void glutDisplayFunc (void (*Display)(void))
   // inregistreaza functia callback Display care este apelata
   // oridecate ori este necesara desenarea ferestrei: la
   // initializare, la modificarea dimensiunilor ferestrei
   // sau la apelul functiei
   // void glutPostRedisplay (void).
   glutDisplayFunc(Display);

   // Functia void glutMainLoop() lanseaza bucla de procesare
   // a evenimentelor GLUT. Din bucla se poate iesi doar prin
   // inchiderea ferestrei aplicatiei. Aceasta functie trebuie
   // apelata cel mult o singura data in program. Functiile
   // callback trebuie inregistrate inainte de apelul acestei
   // functii.
   // Cand coada de evenimente este vida atunci este executata
   // functia callback IdleFunc inregistrata prin apelul functiei
   // void glutIdleFunc (void (*IdleFunc) (void))
   glutMainLoop();

   return 0;
}
