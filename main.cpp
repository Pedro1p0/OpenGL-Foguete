// #include <windows.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

static double x = 0.0, x1 = 0.0, y1 = 0.1, z1 = 0.0, a1 = 0, a2 = 0, y2 = 0, z2 = 0;
static double move = -60;
static bool seperate = false;

static float scale_factor = 1.0;
static bool increasing = true;
static GLfloat light_angle = 0.0;

GLuint texture;
GLUquadricObj *pSphere = NULL;

/**
 * @brief Carrega uma imagem como textura
 * @param texture
 * @param filename
 */
void loadTexture(GLuint *texture, char const *filename)
{
  *texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, (SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT));

  if (*texture == false)
  {
    printf("Erro do SOIL: '%s'\n", filename);
  }
  else
  {
    printf("%d \n", *texture);
  }
}

void stroke_output(GLfloat x, GLfloat y, const char *const format, ...)
{
  va_list args;
  char buffer[200];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  glPushMatrix();
  glTranslatef(x, y, 0);
  glScaled(0.003 * scale_factor, 0.005 * scale_factor, 0.005 * scale_factor);
  // Desative a iluminação antes de definir a cor do material
  glDisable(GL_LIGHTING);
  // Define a cor do material para o texto
  GLfloat mat_diffuse[] = {1, 1, 1, 1.0f};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  for (const char *p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
  glEnable(GL_LIGHTING);
  // Lógica para controlar a animação de escala
  if (increasing)
  {
    scale_factor += 0.001;
    if (scale_factor > 1.2)
    {
      increasing = false;
    }
  }
  else
  {
    scale_factor -= 0.001;
    if (scale_factor < 1.0)
    {
      increasing = true;
    }
  }
}

void drawRoundedCylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks)
{
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluCylinder(quadric, radius, radius, height, slices, stacks);
  gluDeleteQuadric(quadric);
}

void satellite()
{

  glPushMatrix();

  // Core

  glPushMatrix();
  glColor3f(1, 0.8, 0.5);
  glTranslatef(0, 2, 0);
  glScaled(0.3, 1, 0.3);
  glRotatef(90, 1, 0, 0);
  if (x <= 6)
    glutSolidTorus(0.5, 1, 30, 30);
  else
    drawRoundedCylinder(0.6, 1.2, 30, 30);
  glPopMatrix();

  // Solar Panel

  glPushMatrix();
  glColor3f(0.2, 0.2, 0.2);
  glTranslatef(1, 2, 0);

  if (x >= 6)
  {
    glScaled(4, 1.5, 0.1);
  }
  else
    glScaled(0, 0, 0);

  glRotatef(-20, 1, 0, 0);
  glutSolidCube(0.5);
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.2, 0.2, 0.2);
  glTranslatef(-1, 2, 0);
  if (x >= 6)
  {
    glScaled(4, 1.5, 0.1);
  }
  else
    glScaled(0, 0, 0);
  glRotatef(-20, 1, 0, 0);
  glutSolidCube(0.5);
  glPopMatrix();

  glPopMatrix();
}

void rocket()
{

  // Cone superior do foguete
  glPushMatrix();
  // if para ejetar parte superior do foguete
  if (x >= 5.5)
  {
    glTranslatef(z2, -z2, z2);
    glRotatef(a1, 0, 1, 1);
  }
  glPushMatrix();
  glTranslatef(0, 2.5, 0);
  glColor3f(0, 0, 1);
  glScaled(2.2, 1.5, 2.2);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);

  glPopMatrix();

  glPopMatrix();

  // Satellite no container do foguete
  glPushMatrix();
  // Separação do satellite do foguete
  if (x >= 6.8)
  {

    glTranslatef(2, 0, 0);
    glRotatef(x * 30, 0, 1, 0);
  }
  satellite();
  glPopMatrix();

  // inicio parte principal
  glPushMatrix();
  // Separação da parte principal do foguete
  if (x >= 5)
  {
    glTranslatef(0, y2, y2);
    glRotatef(a1, 0, 1, 1);
  }

  // Parte principal do foguete
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 0.0, 0);
  glScaled(0.3, 4.3, 0.3);
  glRotatef(90, 1, 0, 0);
  glutSolidTorus(0.5, 1, 30, 30);
  glPopMatrix();

  // parte inferior do foguete
  glPushMatrix();
  glTranslatef(0, -2.2, 0);
  glColor3f(0.2, 0.2, 0.2);
  glScaled(3, 1.5, 3);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);
  glPopMatrix();

  glPopMatrix();

  // Parte direita do foguete
  // rotaçao do booster
  glPushMatrix();
  glTranslatef(x1, -y1, z1);
  glRotatef(a1, 0, 1, 1);

  // cone superior do booster direito
  glPushMatrix();
  glTranslatef(0.7, 1, 0);
  glColor3f(0, 0, 1);
  glScaled(1.5, 1, 1.5);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);
  glPopMatrix();

  // parte principal do booster direito
  glPushMatrix();
  glTranslatef(0.7, -0.2, 0);
  glColor3f(0, 1, 1);
  glScaled(0.2, 6.5, 0.2);
  glRotatef(90, 1, 0, 0);
  glutSolidTorus(0.2, 1, 30, 30);
  glPopMatrix();

  // cone inferior do booster direito
  glPushMatrix();
  glTranslatef(0.7, -2.0, 0);
  glColor3f(0, 0, 1);
  glScaled(1.5, 1, 1.5);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);
  glPopMatrix();

  glPopMatrix();

  // Parte esquerda do foguete
  // rotação do booster esquerdo
  glPushMatrix();
  glTranslatef(-x1, -y1, -z1);
  glRotatef(-a1, 0, 1, 1);
  // cone superior do booster esquerdo
  glPushMatrix();
  glTranslatef(-0.7, 1, 0);
  glColor3f(0, 0, 1);
  glScaled(1.5, 1, 1.5);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);
  glPopMatrix();

  // corpo do booster esquerdo
  glPushMatrix();
  glTranslatef(-0.7, -0.2, 0);
  glColor3f(0, 1, 1);
  glScaled(0.2, 6.5, 0.2);
  glRotatef(90, 1, 0, 0);
  glutSolidTorus(0.2, 1, 30, 30);
  glPopMatrix();

  // cone inferior do booster esquerdo
  glPushMatrix();
  glTranslatef(-0.7, -2.0, 0);
  glColor3f(0, 0, 1);
  glScaled(1.5, 1, 1.5);
  glRotatef(270, 1, 0, 0);
  glutSolidCone(0.2, 1, 30, 30);
  glPopMatrix();

  // booster
  /*
    glPushMatrix();
    glTranslatef(2,0,0);
    glColor3f(1,1,0);
    glBegin(GL_POLYGON);

    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(0.8,-1,0);
    glVertex3f(0,0,0);
    glEnd();
    glPopMatrix();
  */
  glPopMatrix();
}

void stars()
{

  for (float s1 = 5; s1 <= 100; s1 += 0.4)
  {
    for (float s2 = -6; s2 <= 6; s2 += 0.5)
    {
      glPushMatrix();
      glBegin(GL_POINTS);
      glVertex3f(s2, s1, 0);
      glEnd();
      glPopMatrix();
    }
  }
  for (float s3 = 5.2; s3 <= 100; s3 += 0.3)
  {
    for (float s4 = -6.2; s4 <= 6; s4 += 0.3)
    {
      glPushMatrix();
      glBegin(GL_POINTS);
      glVertex3f(s4, s3, 0);
      glEnd();
      glPopMatrix();
    }
  }
}

// Draw rocket

void draw(double ang)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -13.0f);
  // glRotatef(ang,0.0f,1.0f,0.0f);

  glPushMatrix();

  if (ang <= 2)
  {
    glRotatef(ang * 30, 1, 0, 0);
    glTranslatef(0, -2 + ang, 0);
  }
  else
  {
    glRotatef(60, 1, 0, 0);
    glTranslatef(0, 0, 0);
  }
  glScaled(0.5, 0.5, 0.5);
  rocket();
  glPopMatrix();

  // Terra

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glColor3f(1.0, 1.0, 1.0);
  if (x >= 6.5)
  {
    glTranslatef(0, -18, -95);
    glRotatef(-90.0, 1, 0, 0); // deixa o eixo de rotação da Terra na vertical
    glRotatef(10 * x, 0, 0, 1);
  }
  else
  {
    glTranslatef(0, -10 - x, -10 - 15 * x);
  }

  gluSphere(pSphere, 10, 100, 100);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // Create Stars
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, -ang, 0);
  stars();
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

void callback()
{
  x += 0.01;

  if (x >= 3)
  {
    x1 += 0.1;
    y1 += 0.1;
    z1 += 0.01;
    a1 += 3;
  }

  if (x > 5)
  {
    y2 -= 0.1;
  }

  if (x > 5.5)
  {
    z2 += 0.1;
    a2 += 0.1;
  }
  light_angle += 0.01; // Aumenta o ângulo da luz para simular o movimento do sol

  draw(x);
}

void doInit()
{

  /* Background and foreground color */
  glClearColor(0.0, 0.0, 0.0, 0);
  glViewport(0, 0, 640, 480);

  /* Select the projection matrix and reset it then
     setup our view perspective */

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 200.0f);

  /* Select the modelview matrix, which we alter with rotatef() */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearDepth(2.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void display()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -13.0f);

  stroke_output(-4.0, 1.7, "Projeto final de Computacao grafica");

  stroke_output(-4.0, 0.9, "Para iniciar pressione P");

  GLfloat mat_ambient[] = {0.0f, 1.0f, 2.0f, 1.0f};
  GLfloat mat_diffuse[] = {0.0f, 1.5f, .5f, 1.0f};
  GLfloat mat_specular[] = {5.0f, 1.0f, 1.0f, 1.0f};
  GLfloat mat_shininess[] = {50.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  GLfloat lightIntensity[] = {1.7f, 1.7f, 1.7f, 1.0f};
  GLfloat light_position3[] = {0.0f, 5.0f, 0.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position3);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

  glEnable(GL_COLOR_MATERIAL);

  glFlush();
  glutSwapBuffers();
}

void idle()
{
  glutPostRedisplay();
}

void menu(int id)
{
  switch (id)
  {

  case 2:
    glutIdleFunc(callback);
    break;

  case 5:
    exit(0);
    break;
  }
  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}

void mykey(unsigned char key, int x, int y)
{

  if (key == 'p')
  {
    glutIdleFunc(callback);
  }

  if (key == 'q' || key == 'Q')
  {
    exit(0);
  }
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1000, 720);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("rocket");
  char const *fileName = "./earth_texture.jpg";
  loadTexture(&texture, fileName);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  pSphere = gluNewQuadric();              // cria novo objeto quadric
  gluQuadricDrawStyle(pSphere, GLU_FILL); // define tipo de desenho, nesse caso poligonos primitivos
  gluQuadricNormals(pSphere, GLU_SMOOTH); // normais geradas para cada vértice do quadric
  gluQuadricTexture(pSphere, GLU_TRUE);   // gera coordenadas de textura
  glutKeyboardFunc(mykey);
  glutCreateMenu(menu);
  glutAddMenuEntry("Launch       'p'", 1);

  glutAddMenuEntry("Quit            'q'", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  doInit();
  glutMainLoop();
  return 0;
}
