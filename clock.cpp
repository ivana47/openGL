#include <GL/glut.h>
#include <ctime>
#include <iostream>


int windowWidth = 800;   // sirina prozora
int windowHeight = 800;  // visina prozora

GLfloat i = 0.0;  // second 
GLfloat j = 0.0;  // minute 
GLfloat k = 0.0;  // hour 

GLfloat frameScale = 1.0;  

GLfloat translateX = 0.0;  // pomjeranje
GLfloat translateY = 0.0;  

void initGL() { glClearColor(0.0, 0.0, 0.0, 1.0); }

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  // koordinatni 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.2, 1.2, -1.2, 1.2, -1, 1);  // lijevo,desno,gore,dolje
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glTranslatef(translateX, translateY, 0.0); 

  // provjera jel unutar prozora
if ((frameScale <= windowWidth) && (frameScale <= windowHeight)) {

    // ombre KONACNO
    
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glVertex2f(-0.35 * frameScale, 0.35 * frameScale);
    glVertex2f(0.35 * frameScale, 0.35 * frameScale);
    glColor3f(0, 0, 1);
    glVertex2f(0.4 * frameScale, 0.4 * frameScale);
    glVertex2f(-0.4 * frameScale, 0.4 * frameScale);

    glColor3f(1, 1, 1);
    glVertex2f(0.35 * frameScale, 0.35 * frameScale);
    glVertex2f(0.35 * frameScale, -0.35 * frameScale);
    glColor3f(0, 0, 1);
    glVertex2f(0.4 * frameScale, -0.4 * frameScale);
    glVertex2f(0.4 * frameScale, 0.4 * frameScale);

    glColor3f(1, 1, 1);
    glVertex2f(-0.35 * frameScale, -0.35 * frameScale);
    glVertex2f(-0.35 * frameScale, 0.35 * frameScale);
    glColor3f(0, 0, 1);
    glVertex2f(-0.4 * frameScale, 0.4 * frameScale);
    glVertex2f(-0.4 * frameScale, -0.4 * frameScale);

    glColor3f(1, 1, 1);
    glVertex2f(0.35 * frameScale, -0.35 * frameScale);
    glVertex2f(-0.35 * frameScale, -0.35 * frameScale);
    glColor3f(0, 0, 1);
    glVertex2f(-0.4 * frameScale, -0.4 * frameScale);
    glVertex2f(0.4 * frameScale, -0.4 * frameScale);

    glEnd();
  }

  time_t currentTime = time(NULL);
  struct tm *localTime = localtime(&currentTime);

  glPushMatrix();

  glRotatef(-localTime->tm_sec * 6.0f, 0.0, 0.0,
            0.1);  
  glLineWidth(1.8);
  glBegin(GL_LINES);
  glColor3f(0.5, 0.5, 0.5);  // second 
  glVertex2f(0.0, 0.0);
  glVertex2f(0.0, 0.3 * frameScale);
  glEnd();
  glPopMatrix();

  glPushMatrix();

  glRotatef(-localTime->tm_min * 6.0f, 0.0, 0.0,
            0.1); 
  glLineWidth(2.8);
  glBegin(GL_LINES);
  glColor3f(1.0, 1.0, 0.0);  // minute 
  glVertex2f(0.0, 0.0);
  glVertex2f(0.0, 0.25 * frameScale);
  glEnd();
  glPopMatrix();

  glPushMatrix();

  glRotatef(-(localTime->tm_hour % 12 + localTime->tm_min / 60.0) * 30.0f, 0.0,
            0.0, 0.1);  
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glColor3ub(120, 255, 255);  // hour 
  glVertex2f(0.0, 0.0);
  glVertex2f(0.0, 0.18 * frameScale);
  glEnd();
  glPopMatrix();

  glFlush();
}

void timer(int value) {
  glutPostRedisplay();
  glutTimerFunc(1000, timer, 0);  // upadete svake 1 sek
}

void mouse(int button, int state, int x, int y) {
  std::cout << "mouse: " << x << " " << y << std::endl;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    frameScale += 0.02;

    // Proverite da li veličina prelazi granice prozora
    if ((frameScale > 1.2) || (frameScale > 1.2)) {
      frameScale -= 0.02;  // Vratite prethodnu veličinu
    }

    // Proverite da li sat prelazi granice prozora
    if ((translateX + 0.4 * frameScale > 1.2) ||
        (translateX - 0.4 * frameScale < -1.2) ||
        (translateY + 0.4 * frameScale > 1.2) ||
        (translateY - 0.4 * frameScale < -1.2)) {
      frameScale -= 0.02;  // Vratite prethodnu veličinu
    }
  }
}

void keyboard(unsigned char key, int x, int y) {
  std::cout << "keyboard callback" << std::endl;
  switch (key) {
    case 27:    // ako kliknemo esc to je inace 27
      exit(0);  // tad ugasi aplikaciju
      break;
    case 'r':
    case 'R':
      translateX = 0.0f;
      translateY = 0.0f;
      frameScale = 1.0f;
      break;  
  }
}

void specialKeyboard(int key, int x, int y) {
  std::cout << "Tasstatura, pomjeranje" << std::endl;
  GLfloat previousTranslateX =
      translateX;  // sacuva prethodne pomeranje po x-osi
  GLfloat previousTranslateY =
      translateY;  // i po y-osi

  switch (key) {
    case GLUT_KEY_UP:
      translateY += 0.1;
      break;
    case GLUT_KEY_DOWN:
      translateY -= 0.1;
      break;
    case GLUT_KEY_LEFT:
      translateX -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      translateX += 0.1;
      break;
  }

  // provera da li pomeranje prelazi granice prozora
  //-1.2 sam gore podesila
  if ((translateX + 0.4 * frameScale > 1.2) ||
      (translateX - 0.4 * frameScale < -1.2) ||
      (translateY + 0.4 * frameScale > 1.2) ||
      (translateY - 0.4 * frameScale < -1.2)) {
    translateX = previousTranslateX;  // vrati prethodno pomjeranje po x-osi
    translateY = previousTranslateY;  
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutCreateWindow("User input");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);              
  glutSpecialFunc(specialKeyboard);  
  glutKeyboardFunc(keyboard);
  initGL();
  glutTimerFunc(0, timer, 0);  // timer 
  glutMainLoop();
  return 0;
}

