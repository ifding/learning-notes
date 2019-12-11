#ifndef GRAPHICS_H
#define GRAPHICS_H

#define  KEY_LEFT       0x0164
#define  KEY_UP         0x0165
#define  KEY_RIGHT      0x0166
#define  KEY_DOWN       0x0167

void set_color(double r, double g, double b);
void draw_pixel (int x, int y); 
void draw_line (double x1, double y1, double x2, double y2); 

int init_graphics(int argc, char **argv,
		  int window_xsize, int window_ysize,
		  void (* render)(void),
		  void (* keyhandler)(int key));

#endif
