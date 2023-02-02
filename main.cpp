////#define _USE_MATH_DEFINES
//#include <string>
//#include <iostream>
//#include <stdlib.h>
//using namespace std;
////#include <GL/glut.h>
////#include <math.h>
//
////
////#include "Helper.h"
////
////int WIN_HEIGHT = 400;
////int WIN_WIDTH = 400;
////
//////const char* string = ;
////void text(void);
////
////
////int main(int argc, char** argv) {
////    glutInit(&argc, argv);
////    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
////    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
////    glutInitWindowPosition(100, 100);
////    glutCreateWindow("Scoreboard");
////    glClearColor(0.0, 0.0, 0.0, 1.0);
////    glMatrixMode(GL_PROJECTION);
////    glLoadIdentity();
////    glOrtho(0.0f, 1000, -1200.0f, 100.0f, 1.0f, 0.0f);
////    glutDisplayFunc(&text);
////    glutMainLoop();
////    return(0);
////}
////
////void text(void) {
////
////    glClear(GL_COLOR_BUFFER_BIT);
////    glPushMatrix();
////    for (const char* p = "Scoreboard"; *p; p++)
////        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
////    glPopMatrix();
////    glFlush();
////}
//
//
//// For player class
//class Player
//{
//private:
//	std::string name;
//	float average;
//	char team;
//public:
//	Player(std::string n, float a, char t)
//	{
//		name = n;
//		average = a;
//		team = t;
//	}
//	void setname(std::string n)
//	{
//		name = n;
//	}
//	void setaverage(float a) 
//	{
//		average = a;
//	}
//	void setteam(char t)
//	{
//		team =t;
//	}
//	std::string getname()
//	{
//		return name;
//	}
//	int getaverage()
//	{
//		return average;
//	}
//	char getteam()
//	{
//		return team;
//	}
//	void input(std::string n, float a, char t)
//	{
//		name = n;
//		average = a;
//		team = t;
//	}
//	void Change(std::string n, float a, char t)
//	{
//		name = n;
//		average = a;
//		team = t;
//	}
//	void Display()
//	{
//		cout << "Player Name is: " << name << endl;
//		cout << "Player Average is: " << average << endl;
//		cout << " Player team is: " << team << endl;
//	}
//};
//
//int main()
//{
//	string name;
//	float average;
//	char team;
//	cout << "Enter NAme of the player: " << endl;
//	//cin >> name;
//	cout << "Enter average of the player: " << endl;
//	//cin >> average;
//	cout << " Enter team of the player: " << endl;
//	//cin >> team;
//	Player obj(name, average, team);
//	obj.input(name, average, team);
//	obj.Change(name, average, team);
//	obj.Display();
//
//	return 0;
//
//}
