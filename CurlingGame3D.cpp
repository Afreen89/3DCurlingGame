
// Some user-defined and built-in libraries

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string>

#include "Helper.h"


#include "udp_client.h"


#define	SIM_UPDATE_MS	(10)
#define DRAW_SOLID (0)

 //Object for CurlingSheet
LongCurlingSheet sheet;



Client player_client;

//variables for window
int gameWindowWidth = 900;
int gameWindowHeight = 900;

//camera variables
vec3 gCamPos(0.0, -0.7, 2);
vec3 gCamLookAt(0.0, 0.5, -1.0);
bool gCamRotate = true;
float gCamRotSpeed = 0.1;
float gCamMoveSpeed = 0.1;
bool gCamL = false;
bool gCamR = false;
bool gCamU = false;
bool gCamD = false;
bool gCamZin = false;
bool gCamZout = false;


std::string teamAScore = "Score: 0"; 
std::string teamBScore = "Score: 0";
std::string winner = "";
std::string myRemainStones = "";
std::string otherRemainStones = "";

bool TeamTurn = true;

void StonesRemain()
{
    int remaining = 0;
    for (size_t i = 0; i < sheet.my_team.stones.size(); i++)
    {
        if (sheet.my_team.stones.at(i).is_draw && !sheet.my_team.stones.at(i).is_moving)
        {
            remaining += 1;
        }
    }


    remaining = NUM_BALLS - remaining;
    
    player_client.player.stones_to_hit = remaining;
    myRemainStones = "Stones: ";
    for (int i = 0; i < remaining; i++)
    {
        myRemainStones +=   "* ";
    }
  
    otherRemainStones = "Stones: ";
    
    for (int i = 0; i < player_client.other_player.stones_to_hit; i++)
    {
        otherRemainStones += "* ";
    }


}

bool DecideTurn(void)
{
    int count_drawn = 0;
    for (size_t i = 0; i < sheet.my_team.stones.size(); i++)
    {
        if (sheet.my_team.stones.at(i).is_draw && !sheet.my_team.stones.at(i).is_moving)
        {
            count_drawn += 1;
        }
    }

    for (size_t i = 0; i < sheet.other_team.stones.size(); i++)
    {
        if (sheet.other_team.stones.at(i).is_draw && !sheet.other_team.stones.at(i).is_moving)
        {
            count_drawn += 1;
        }
    }

//    std::cout << "Count Draw: " << count_drawn << std::endl;
    if (player_client.player.id == 0)
    {
        return count_drawn % 2 == 0;
    }
    else
    {
        return count_drawn % 2 == 1;
    }

}

void DecideWinner(void)
{
    int count_drawn = 0;
    for (size_t i = 0; i < sheet.my_team.stones.size(); i++)
    {
        if (sheet.my_team.stones.at(i).is_draw && !sheet.my_team.stones.at(i).is_moving ) 
        {
            count_drawn += 1;
        }
    }

    for (size_t i = 0; i < sheet.other_team.stones.size(); i++)
    {
        if (sheet.other_team.stones.at(i).is_draw && !sheet.other_team.stones.at(i).is_moving)
        {
            count_drawn += 1;
        }
    }

    if (count_drawn == NUM_BALLS * 2)
    {
        std::string win = " is the winner";
        if (player_client.player.score > player_client.other_player.score)
        {
            winner = player_client.player.team_name + win;
        }
        else if (player_client.player.score < player_client.other_player.score)
        {
            winner = player_client.other_player.team_name + win;
        }
        else
        {
            winner = "Match Tie!";
        }
    }

}



void RenderFont(float x, float y, char* text)
{
    char* c;
//    glRasterPos3f(x, y, z);
    glRasterPos2f(x, y);
    for (c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void RenderFont(float x, float y, std::string text)
{
    char* c;

    char char_arr[255];
    strcpy(char_arr, text.c_str());

    glRasterPos2f(x, y);
    for (c = char_arr; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}


void DoCamera(int ms)
{
    static const vec3 up(0.0, 1.0, 0.0);

    if (gCamRotate)
    {
        if (gCamL)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localL = up.Cross(camDir);
            vec3 inc = (localL * ((gCamRotSpeed * ms) / 1000.0));
            gCamLookAt = gCamPos + camDir + inc;
        }
        if (gCamR)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = up.Cross(camDir);
            vec3 inc = (localR * ((gCamRotSpeed * ms) / 1000.0));
            gCamLookAt = gCamPos + camDir - inc;
        }
        if (gCamU)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = camDir.Cross(up);
            vec3 localUp = localR.Cross(camDir);
            vec3 inc = (localUp * ((gCamMoveSpeed * ms) / 1000.0));
            gCamLookAt = gCamPos + camDir + inc;
        }
        if (gCamD)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = camDir.Cross(up);
            vec3 localUp = localR.Cross(camDir);
            vec3 inc = (localUp * ((gCamMoveSpeed * ms) / 1000.0));
            gCamLookAt = gCamPos + camDir - inc;
        }
    }
    else
    {
        if (gCamL)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localL = up.Cross(camDir);
            vec3 inc = (localL * ((gCamMoveSpeed * ms) / 1000.0));
            gCamPos += inc;
            gCamLookAt += inc;
        }
        if (gCamR)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = camDir.Cross(up);
            vec3 inc = (localR * ((gCamMoveSpeed * ms) / 1000.0));
            gCamPos += inc;
            gCamLookAt += inc;
        }
        if (gCamU)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = camDir.Cross(up);
            vec3 localUp = localR.Cross(camDir);
            vec3 inc = (localUp * ((gCamMoveSpeed * ms) / 1000.0));
            gCamPos += inc;
            gCamLookAt += inc;
        }
        if (gCamD)
        {
            vec3 camDir = (gCamLookAt - gCamPos).Normalised();
            vec3 localR = camDir.Cross(up);
            vec3 localDown = camDir.Cross(localR);
            vec3 inc = (localDown * ((gCamMoveSpeed * ms) / 1000.0));
            gCamPos += inc;
            gCamLookAt += inc;
        }
    }

    if (gCamZin)
    {
        vec3 camDir = (gCamLookAt - gCamPos).Normalised();
        vec3 inc = (camDir * ((gCamMoveSpeed * ms) / 1000.0));
        gCamPos += inc;
        gCamLookAt += inc;
    }
    if (gCamZout)
    {
        vec3 camDir = (gCamLookAt - gCamPos).Normalised();
        vec3 inc = (camDir * ((gCamMoveSpeed * ms) / 1000.0));
        gCamPos -= inc;
        gCamLookAt -= inc;
    }
}

// Function for displaying game window
void display(void)
{
    //glClearColor(0.0, 0.0, 0.0, 1.0); //Set background color to blackand opaque
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer (background)
    
        
    glLoadIdentity();
    glPushMatrix();

    gluLookAt(gCamPos(0), gCamPos(1), gCamPos(2), gCamLookAt(0), gCamLookAt(1), gCamLookAt(2), 0.0f, 1.0f, 1.0f);
    
    sheet.Draw();

    //RenderFont(-1.0, 1.0, sheet.my_team.name);
    //RenderFont( 1.0, 1.0, sheet.other_team.name);

    // team names
    RenderFont(-0.69, 1.0, player_client.player.team_name);
    RenderFont(0.3, 1.0, player_client.other_player.team_name);
    
    // winner name
    RenderFont( -0.1, 0.1, winner);
    
    // scores
    RenderFont(-0.69, 0.90, teamAScore);
    RenderFont(0.3, 0.90, teamBScore);
    
    // stones remainng
    RenderFont(-0.69, 0.80, myRemainStones);
    RenderFont(0.3, 0.80, otherRemainStones);



   // glColor3f(1.0f, 0.0f, 0.0f);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    glLoadIdentity();
}


void init(void)
{
    // initialize viewing system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);

     //shading model
    glEnable(GL_SMOOTH);

    // enable depth buffering
    glEnable(GL_SMOOTH | GL_DEPTH_TEST);
    glLoadIdentity();
}

//Function for Camera views using keyboard
void KeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    { 
    case(32):{ gCamRotate = false; break;}
    case('z'):{ gCamL = true;break;}
    case('c'):{ gCamR = true; break;}
    case('s'):{ gCamU = true;break;}
    case('x'):{ gCamD = true; break;}
    case('f'):{ gCamZin = true;break;}
    case('v'):{ gCamZout = true;break;}
    case('m'):{ std::cout << "you are pressing 'm'" << std::endl;sheet.Reset();break;}
    case('k'):
        {
            if (DecideTurn())
            {
                std::cout << "you are pressing 'k'" << std::endl;
                float updated_x = sheet.arrow.get_length() * cos(sheet.arrow.get_direction());
                float updated_y = sheet.arrow.get_length() * sin(sheet.arrow.get_direction());
                vec2 updated = { updated_x, updated_y };
                updated *= 150;
                for (int i = 0; i < NUM_BALLS; i++)
                {
                    // first check if any teams balls are moving or not!
                    if (sheet.my_team.stones.at(i).is_moving || sheet.other_team.stones.at(i).is_moving)
                    {
                        break;
                    }

                    // if its team A turn, apply a force on team A ball.
                    if (!sheet.my_team.stones.at(i).is_draw)
                    {
                        sheet.my_team.stones.at(i).original_force_applied.elem[0] = updated.elem[0];
                        sheet.my_team.stones.at(i).original_force_applied.elem[1] = updated.elem[1];
                        sheet.my_team.stones.at(i).SetForce(updated);
                        sheet.my_team.stones.at(i).is_draw = true;
                        player_client.player.score += 1;

                     /*   if (player_client.other_player.ball_positions_y[i] > 0.48)
                        {
                            player_client.player.score += 1;
                        }*/

                                            
                        break;
                    }
                }

            }
            break;
        }
    default:
        {std::cout << "you pressed an 'unknown key'" << std::endl;break;}   
    }
}

void KeyboardUpFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case(32):{ gCamRotate = true;break;}
	case('z'):{ gCamL = false;break;}
	case('c'):{ gCamR = false;break;}
	case('s'):{ gCamU = false;break;}
	case('x'):{ gCamD = false;break;}
	case('f'):{ gCamZin = false;break;}
	case('v'):{ gCamZout = false;break;}
	}
}

//Function for keyboard keys for user to play
void SpeckeyboardFunc(int key, int x, int y)
{
    switch (key)
    {

    case GLUT_KEY_LEFT:
    {
        std::cout << "You are changing the direction of the arrow to left" << std::endl;
        if (DecideTurn())
        {
            sheet.arrow.set_direction(sheet.arrow.get_direction() + 0.025);
        }

        break;
    }
    case GLUT_KEY_RIGHT:
    {
        std::cout << "You are changing the direction of the arrow to right" << std::endl;
        if (DecideTurn())
        {
            sheet.arrow.set_direction(sheet.arrow.get_direction() - 0.025);
        }
        break;
    }
    case GLUT_KEY_UP:
    {
        std::cout << "You are increasing the length of the arrow" << std::endl;
        if (DecideTurn())
        {
            sheet.arrow.set_length(sheet.arrow.get_length() + 0.025);
        }break;
    }
    case GLUT_KEY_DOWN:
    {
        std::cout << "You are decreasing the length of the arrow" << std::endl;
        if (DecideTurn())
        {
            sheet.arrow.set_length(sheet.arrow.get_length() - 0.025);
        }break;
    }

    }
}

void ChangeSize(int w, int h) {

     //Prevent a divide by zero, when window is too short
     //(you cant make a window of zero width).
    if (h == 0) h = 1;
    float ratio = 1.0 * w / h;

     //Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, ratio, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


 //Function for updating game window
void update(int ms)
{
    
    sheet.Update(ms);

    DoCamera(ms);
      
    DecideWinner();
    StonesRemain();

    if (DecideTurn())
    {
        if ( player_client.player.id == 0 )
            strcpy(player_client.player.team_name, "Team A*");
        else
            strcpy(player_client.player.team_name, "Team B*");


        player_client.player.arrow_direction = sheet.arrow.get_direction();
        player_client.player.arrow_length = sheet.arrow.get_length();
    }
    else
    {
        if (player_client.player.id == 0)
            strcpy(player_client.player.team_name, "Team A");
        else
            strcpy(player_client.player.team_name, "Team B");

        sheet.arrow.set_direction(player_client.other_player.arrow_direction);
        sheet.arrow.set_length(player_client.other_player.arrow_length);
    }

    player_client.player.score = sheet.my_team.score;
    teamAScore = "Score\n: " + std::to_string(player_client.player.score);
    teamBScore = "Score\n: " + std::to_string(player_client.other_player.score);

    for (size_t i = 0; i < NUM_BALLS; i++)
    {
        player_client.player.ball_positions_x[i] = sheet.my_team.stones.at(i).original_force_applied.elem[0];
        player_client.player.ball_positions_y[i] = sheet.my_team.stones.at(i).original_force_applied.elem[1];
        player_client.player.ball_status[i] = sheet.my_team.stones.at(i).is_draw; // ? 1 : 0;
        
        float x = player_client.other_player.ball_positions_x[i];
        float y = player_client.other_player.ball_positions_y[i];
        sheet.other_team.stones.at(i).is_draw = player_client.other_player.ball_status[i]; // == 1 ? true : false;

        if (sheet.other_team.stones.at(i).is_draw && !sheet.other_team.stones.at(i).is_rendered )
        {

           // std::cout << "Applied force of x: " << x << "\ty: " << y << std::endl;
            sheet.other_team.stones.at(i).is_rendered = true;
            sheet.other_team.stones.at(i).SetForce(vec2(x, y));
        }

    }
    
    glutTimerFunc(SIM_UPDATE_MS, update, SIM_UPDATE_MS);
    glutPostRedisplay();
}

 //main function necessary to run the program
int main(int argc, char** argv)
{
      
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <client-id> <server-ip> <server-port>" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Player is a UDP client!\n";
        player_client.Start(atoi(argv[1]), argv[2], argv[3]);
        player_client.InteractThreaded();
    }
    
    bool is_solid = atoi(argv[1]) == 0;
    vec3 dim = vec3(0.5, 0.7, 0.05);
    sheet = LongCurlingSheet(dim, 0, is_solid);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(gameWindowWidth, gameWindowHeight);

    if (atoi(argv[1]) == 0)
    {
        glutInitWindowPosition(0, 0);
    }
    else
    {
        glutInitWindowPosition(900, 0);
    }

    // Curling sheet name
    glutCreateWindow("MSc Project - Curling");

    glLoadIdentity();
    init();

    // Setting the stage. 
    glutDisplayFunc(display);

    // Updating the changes. 
    glutTimerFunc(SIM_UPDATE_MS, update, SIM_UPDATE_MS);
    glutReshapeFunc(ChangeSize);
    glutIdleFunc(display);

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpeckeyboardFunc);
    glutKeyboardUpFunc(KeyboardUpFunc);
    
    glEnable(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();

    player_client.Stop();
    return 1; 

}
