#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "texheader.h"

GLint   windowWidth  = 500;  //Display window dimensions
GLint   windowHeight = 500;
GLfloat light_position[]= {10.0, 5.0, 10.0, 1.0};
GLfloat rotate= 0.0;

TextureImage tx[1];         // Storage For One Texture

void LoadGLTexture(void) {
  // Load Bitmaps And Convert To Textures
  LoadTGA(&tx[0], "self-portrait-1.tga");
  glGenTextures(1, &tx[0].texID); 	// Create a Texture object
  glBindTexture(GL_TEXTURE_2D, tx[0].texID);

  glTexImage2D(GL_TEXTURE_2D, 0, 3, tx[0].width, tx[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE, tx[0].imageData);
  //printf(" width= %d, height = %d \n",TextureImage->sizeX,TextureImage->sizeY); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glEnable(GL_TEXTURE_2D);
}

void reshape(GLint width, GLint height) {
  if(width==0) width=1;
  
  glViewport(0,0, width, height);
  
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective(45.0, (float)width/(float)height, 0.1, 400.0);
  glMatrixMode( GL_MODELVIEW );
}

void init(void) {
  LoadGLTexture();
  // define the camera location and view angle
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glMatrixMode(GL_MODELVIEW);
}

void animate(void) {
  rotate+=0.5;
  if(rotate > 360) rotate-= 360;

  glClearColor(360/rotate, 360/rotate, 360/rotate, 0.0);

  glutPostRedisplay();
}

int main(GLint argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  glutInitWindowSize(windowWidth,windowHeight);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Man Caught in Trash Compactor");
  
  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(animate);
  glutMainLoop();
  return 0;
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity(); 
  gluLookAt(0,0,5, 0,0,0,  0,1,0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glRotatef(rotate, 0,1,0);
  GLUquadric *quad= gluNewQuadric();
  gluQuadricTexture(quad, tx[0].texID);
//   glTranslatef(0,0.375,0);
  glScalef(1.0, 1.5, 1.0);
  glRotated(90,1,0,0);
//   gluCylinder(quad, 1,1,3,24,24);
  gluSphere(quad, 1.5, 24, 24);
  gluDeleteQuadric(quad);

  glFlush();
  glutSwapBuffers();

}
