#ifndef _tex_header
#define _tex_header

typedef struct            // Create A Structure
{
  GLubyte *imageData;          // Image Data (Up To 32 Bits)
  GLuint bpp;           // Image Color Depth In Bits Per Pixel.
  GLuint width;           // Image Width
  GLuint height;           // Image Height
  GLuint texID;           // Texture ID Used To Select A Texture
} TextureImage;            // Structure Name

typedef enum Material {RED_TRANSPARENT, YELLOW_TRANSPARENT, GREEN_TRANSPARENT,
		       LIGHT_GRAY_SOLID, DARK_GRAY_SOLID, BLUE_SOLID, YELLOW_SOLID, BLACK_SOLID};

// Unit pieces
void pillar(void);
void light(void);
void wingStrut(void);
void triangleLegoUnit(GLint wide, GLint deep, GLfloat height);
void legoUnit(GLint wide, GLint deep, GLfloat height, bool hasBumps);
void steeringWheel(void);
void noseCone(void);
void subThruster(void);
void thrusterFin(void);
void thrusterFinSide(void);
void mainThruster(void);
void thrusterMount(void);

// Lego Man pieces
void legoManLegs(void);
void legoManTorso(void);
void legoManArm(void);
void legoManHead(void);
void legoManHelmet(void);
void legoManBackpack(void);

// Misc support
void display(void);
void reshape(GLint width, GLint height);
void keyboard(unsigned char key, GLint x, GLint y);
void specialKeyboard(GLint, GLint, GLint);
void init(void);
int main(GLint argc, char **argv);
void LoadGLTexture(void);
bool LoadTGA(TextureImage *texture, char *filename);
void calcNormal(GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2,
		GLfloat x3, GLfloat y3, GLfloat z3);
void setMaterial(Material mat);
#endif
