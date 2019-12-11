#include <iostream>
#include "glut.h"
#include "graphics.h"

static int window_id, window_xsize, window_ysize;
static double window_scale = 1.0;
static void (*my_render)(void);
static void (*my_keyhandler)(int key);

static void gr_change_size(int w, int h)
{
  glViewport(0, 0, w, h);
  window_xsize = w;
  window_ysize = h;
}

void gr_set_orthographic_projection( void )
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, window_xsize, 0, window_ysize);
  glScalef(1, -1, 1);
  glTranslatef(0, -window_ysize, 0);	
  glMatrixMode(GL_MODELVIEW);
}

void gr_process_normal_keys(unsigned char key, int x, int y)
{
  my_keyhandler(key);
  glutPostRedisplay();
}

void gr_process_special_keys(int key, int x, int y)
{
  my_keyhandler(key | 0x100);
  glutPostRedisplay();		
}

void set_color(double r, double g, double b)
{
  glColor3f(r,g,b);
  
}

void draw_line (double x1, double y1, double x2, double y2)
{
  glBegin(GL_LINES);
  glVertex2f(x1*window_scale,y1*window_scale);
  glVertex2f(x2*window_scale,y2*window_scale);
  glEnd();
}

void draw_pixel (int x, int y)
{
  glBegin(GL_LINES);
  glVertex2f(x,y);
  glVertex2f(x+1,y);
  glEnd();
}

void gr_render( void )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gr_set_orthographic_projection();
  glPushMatrix();
  glLoadIdentity();

  my_render();
 
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

void gr_timerhandler(int v) {

  // glutPostRedisplay();
  // glutTimerFunc(30, gr_timerhandler, 0);
}

int init_graphics(int argc, char **argv,
		  int window_xsize, int window_ysize,
		  void (* render)(void),
		  void (* keyhandler)(int key))
{
  my_render = render;
  my_keyhandler = keyhandler;

  glutInit(&argc, argv);
  glEnable(GL_DEPTH_TEST);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(20,20);
  glutInitWindowSize(window_xsize, window_ysize);
  window_id = glutCreateWindow("Graphics");
	
  glutSetWindow( window_id );
  glutReshapeFunc( gr_change_size ); 
  glutDisplayFunc( gr_render ); 
  glutKeyboardFunc( gr_process_normal_keys );
  glutSpecialFunc( gr_process_special_keys );
  glutTimerFunc(500, gr_timerhandler, 0);
	
  glutMainLoop();
  return 0;
}

