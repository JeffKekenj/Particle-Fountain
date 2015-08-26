#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <Math.h>


#define X 0
#define Y 1
#define Z 2

#define MAX_PARTICLES 100

int running;
int speedIncr;

/*
Jeff Kekenj
4759171
COSC 3P98
Assignment 3
*/

float xpos;
float ypos;
float zpos;

float xspeed=0;					
float yspeed=120;					

struct glob {
   float angle[3];
   int axis;
};

typedef struct {
	//position
   float x;
   float y;
   float z;

   //color
   float r;
   float g;
   float b;

   //direction
   float xi;
   float yi;
   float zi;

   	 //Gravity  
	float   xg;                
	float   yg;                 
	float   zg;

	int	active;					
	float	life;					// Particle Life
	float	fade;					// Fade Speed
} Particles;

	
Particles particle[MAX_PARTICLES]; 

struct glob global= {{0.0,0.0,0.0},Y};

void userintro() {
   printf("Left mouse is rotate left\n");
   printf("Right mouse is rotate right\n");
   printf("Middle mouse = reset\n");
   printf("x, y, z = rotate about x, y, or z axis\n");
   printf("S: Speed increase\n");
   printf("I: Random particle\n");
   printf("R: Reset\n");
   printf("B: Burst/Spray\n");
   printf("q = quit\n");
}


void updateThings(){
	int i;
	int j,k;	
	for (i = 0;i<MAX_PARTICLES;i++){
		//direction movement
		particle[i].z+=particle[i].zi;
		particle[i].y+=particle[i].yi;		
		particle[i].x+=particle[i].xi;
		//gravity
		particle[i].yi+=particle[i].yg;
	}
}

void drawEverything(void) {	
	int busy1, busy2;
  	int loop;
	int i,j;
	int p[][3] = {{15,0,15}, {15,0,15}, {-15,0,15}, {-15,0,15},
                 {15,0,-15}, {15,0,-15}, {-15,0,-15}, {-15,0,-15}};

  int e[][4] = {{0,3,2,1},{3,7,6,2},{7,4,5,6},{4,0,1,5}, {0,4,7,3},{1,2,6,5}};
  float c[][3] = {{1.0,0,0},{0,1.0,0},{1.0,1.0,1.0},
		 {0,0,1.0},{.6,0,.6},{0,.6,.6}};
  int q;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glRotatef(global.angle[X], 1.0, 0.0, 0.0);
  glRotatef(global.angle[Y], 0.0, 1.0, 0.0);
  glRotatef(global.angle[Z], 0.0, 0.0, 1.0);

  for (q=0; q < 6; ++q) {
     glColor3fv(c[q]);
     glBegin(GL_QUADS);
        glVertex3iv(p[e[q][0]]);
        glVertex3iv(p[e[q][1]]);
        glVertex3iv(p[e[q][2]]);
        glVertex3iv(p[e[q][3]]);
     glEnd();
  }  
  	for (loop=0;loop<MAX_PARTICLES;loop++){
		if (particle[loop].active){	
			glColor3f(particle[loop].r,particle[loop].g,particle[loop].b);

			glBegin(GL_QUADS);					
			    glVertex3f(particle[loop].x+0.5f,particle[loop].y+0.5f,particle[loop].z); 
				glVertex3f(particle[loop].x-0.5f,particle[loop].y+0.5f,particle[loop].z); 
				glVertex3f(particle[loop].x+0.5f,particle[loop].y-0.5f,particle[loop].z); 
				glVertex3f(particle[loop].x-0.5f,particle[loop].y-0.5f,particle[loop].z); 
			glEnd();


			particle[loop].x+=particle[loop].xi/3000;
			particle[loop].y+=particle[loop].yi/3000;
			particle[loop].z+=particle[loop].zi/3000;

			particle[loop].x*=speedIncr;
			particle[loop].y*=speedIncr;
			particle[loop].z*=speedIncr;
	
			particle[loop].yi+=particle[loop].yg;			
			particle[loop].life-=particle[loop].fade;		

			for(busy1 = 0; busy1<100;busy1++){
				for(busy2 = 0;busy2<100;busy2++){}
			}//busy loop to slow down animation


			if (particle[loop].y<0){
				particle[loop].y = particle[loop].y * (-1);
			}//bounce

			if (particle[loop].y < -2.0){
				particle[loop].active = 0;
			}//particle dies

			if (particle[loop].y < 1){
				particle[loop].x = (particle[loop].x)*(0.1);
				particle[loop].z = (particle[loop].z)*(0.1);
			}//friction

			if (particle[loop].life<0.0f)					
			{
				particle[loop].life=1.0f;					
				particle[loop].fade=(float)(rand()%100)/1000.0f+0.003f;	
				particle[loop].x=0.0f;						
				particle[loop].y=0.0f;						
				particle[loop].z=0.0f;						
				particle[loop].xi=xspeed+(float)((rand()%60)-32.0f);
				particle[loop].yi=yspeed+(float)((rand()%60)-30.0f);
				particle[loop].zi=(float)((rand()%60)-30.0f);	
				particle[loop].r=0.0f;		
				particle[loop].g=1.0f;			
				particle[loop].b=0.0f;
			}
		}
	}
	glFlush();
	glutSwapBuffers();
}

void makeParticle(){	
	int i;
	for (i = 0;i<MAX_PARTICLES;i++)                 
	{
		particle[i].active = 1;	
		particle[i].life=1.0f;
		particle[i].fade=(float)(rand()%100)/1000.0f+0.003f;	

		particle[i].r = 0.0f;		
		particle[i].g = 1.0f;	
		particle[i].b = 0.0f;	

		particle[i].x = 0.0;
		particle[i].y = 0.0;
		particle[i].z = 0.0;

		particle[i].xi = (float)(((rand()%50)-24.0f));		
		particle[i].yi = (float)((rand()%500+100)*2);
		particle[i].zi = (float)(((rand()%50)-24.0f));	
									
		particle[i].yg = -0.7f;
	}	
}


void startThing(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);		
	drawEverything();  
}///begin everything


void keyboard(unsigned char key, int x, int y) {

   switch (key){
      case 'x':
      case 'X':
	 global.axis = X;
	 break;

      case 'y':
      case 'Y':
	 global.axis = Y;
	 break;

	 case 's':
     case 'S':
	 speedIncr = (rand()%8)+1;
	 break;//speed

	 case 'b':
     case 'B':
	 speedIncr = 1;
	 makeParticle();
	 glutDisplayFunc(startThing);
	 glutIdleFunc(startThing); 
	 break;//spray mode

	 case 'r':
	 case 'R':
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	makeParticle();
	glFlush();
	glutSwapBuffers();
	break;//reset

	case 'i':
	case 'I':
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateThings();
	glFlush();
	glutSwapBuffers();
	break;//reset

      case 'z':
      case 'Z':
	 global.axis = Z;
	 break;

	  case 'p':
      case 'P': 
	 break;

      case 0x1B:
      case 'q':
      case 'Q':

      exit(0);
	 break;
   }
}

void mouse(int btn, int state, int x, int y) {
   if (state == GLUT_DOWN) {
      if (btn==GLUT_LEFT_BUTTON) {
  	  global.angle[global.axis] = global.angle[global.axis] + 0.2;
      }
      else if (btn == GLUT_RIGHT_BUTTON) {
	  global.angle[global.axis] = global.angle[global.axis] - 0.2;
      }
      else {
	  global.angle[X] = 0.0;
	  global.angle[Y] = 0.0;
	  global.angle[Z] = 0.0;
	  glPopMatrix();
	  glPushMatrix();
      }
   }
}

main(int argc, char **argv) {
  userintro();
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Assignment 3");
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  speedIncr = 1;
  makeParticle();
  glutDisplayFunc(startThing);
  glutIdleFunc(startThing); 

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  gluPerspective(70.0, 1.0, 10.0, 75.0);
  gluLookAt(0.0, 20.0, 35.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glutMainLoop();
}