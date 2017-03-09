k#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265
float cols[2][3] = {{0,0,0}, {1,1,1} };

/***
 CAMERA INFORMATION
***/
float pos[] = {0,0,0}; //Camera position
float rot[] = {0, 0, 0}; //Camera rotation

/***
 INITIAL CONDITION
 These conditions are what setup the default terrain for the program
***/
int viewMode = 0; //View mode of the terrain
bool solid = true; //Whether solid terrain is shown
bool wireframe = false; //Whether wireframe terrain is shown
bool both = false; //Whether solid and wireframe terrain is shown
bool clearWire = false;
bool flatview = false;
bool quads = true; //Whether quads or triangles are to be drawn
int sizex = 50; //The X size of the terrain
int sizez = 50; //The Z size of the terrain
float yCoords[301][301]= {};//initially empty
bool lightOn = true; //Whether lighting is on/off
float max = 0; //The peak height

/*** 
 EYE LOCATION
 ***/
float eye[] = {0,5,0}; //The location of where the eye is pointing at
float angle = 0.0f; //The angle of the eye

/***
LIGHTING LOCATIONS
***/
float lightpos1[] = {50,20,-50,1}; //Position of light1
float lightpos2[] = {0,20,0,1}; //Position of light2
	
/* heightMap - takes in the the x and z dimensions of he graph
and the number of iterations that random will be applied.
This generates a height map for the entire graph outputs it
to a global YCoords value

int x - The dimension in the x plane
int z - The dimension in the z plane
int n - The iteration count used to develop heights
*/
void heightMap(int x, int z, int n){
	for(int w = 0;w<n;w++){
	int randx = rand()%x;
	int randz = rand()%z;
	int randr = (rand()%sizex/5)+1;//random radius based on size
	int maxheight = (rand()%(sizex/8))+5;//random max height based on size
	for(int i = 0;i<x;i++){
		for(int j = 0;j<z; j++){
		//below is the circle algorthim that was provided.
		int newx = i - randx;
		int newz = j - randz;
		float d = sqrtf((newx*newx)+(newz*newz));
		float pd = (2*d)/randr;
		if(fabs(pd) <= 1.0){
			yCoords[i][j] += (maxheight/2.0) 
			+ (cos(pd*PI))*(maxheight/2.0);
		}
		}
	}
	}


}
/*getHeight- returns the height of the coordinate in the grid

int x - The dimension in the x plane
int z - The dimension in the z plane

float return - The height at the specified x and z coordinate
*/
float getHeight(int x,int z){
 	return yCoords[x][z];
}

/* drawGrid - takes in the point of orgin of where it will be drawn,
It also takes in the x and z coords, w and l respectfully, to return
a complete grid.

float* c - Point of origin of where the terrain will be drawn
float w - The width in x coordinates of the grid
float l - The length in z cordinates of the grid

 */
void drawGrid(float* c, float w,float l){
	
	glPushMatrix();
	
	glRotatef(rot[1], 0, 1, 0);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[2], 0, 0, 1);
	for (int i = 1; i< w+1; i++){
		for(int j = 1;j<l+1; j++){
	if(quads){//if quads is true, then draw it with quads
	glBegin(GL_QUADS);
	}
	else{
	glBegin(GL_TRIANGLES);//else, draw it with triangles
	}
	float center = getHeight(i,j); //used for flat shading
	float bl = getHeight(i,j);//bottom left vertice
	float tl = getHeight(i,j);//top left vertice
	float tr = getHeight(i,j);//top right vertice
	float br = getHeight(i,j);//bottom right vertice
	if(i != 1){//this is the left
	bl += getHeight(i-1,j);
	tl += getHeight(i-1,j);
	}
	if(i != w+1){//this is the right
	tr += getHeight(i+1,j);
	br += getHeight(i+1,j);
	}
	if(j != 1){//this is the bottom
	bl += getHeight(i,j+1);
	br += getHeight(i,j+1);
	}
	if(i != l+1){//this is the top
	tl += getHeight(i,j-1);
	tr += getHeight(i,j-1);
	}
	if(i != 1 && j!=1){//this is the left bottom
	bl += getHeight(i-1,j+1);
	}
	if(i != 1&& j!=l+1){//this is the left top
	tl += getHeight(i-1,j-1);
	}
	if(i != w+1 && j!=l+1){//this is the right top
	tr += getHeight(i+1,j-1);
	}
	if(i != w+1 && j!=1){//this is the right bottom
	br += getHeight(i+1,j+1);
	}
	//takes the average of the values and compares them with the 
	//max value, if it is higher, then it replaces the max value with
	//the new value
	bl = bl/4;
	if(bl>max){
	max = bl;
	}
	tl = tl/4;
	if(tl>max){
	max = tl;
	}
	tr = tr/4;
	if(tr>max){
	max = tr;
	}
	br = br/4;
	if(br>max){
	max = br;
	}	
	//condences information
	float v[4][3] = { {c[0]+i,c[1]+bl, c[2]-j-1},
			{c[0]+i,c[1]+tl, c[2]-j},
			{c[0]+i+1,c[1]+tr, c[2]-j},
			{c[0]+i+1,c[1]+br, c[2]-j-1}};
	//if it is gondar shading, proceed
	if(!flatview){
		//if the wireframe hasnt yet been drawing, draw the
		//solid view
		if(!clearWire){
		
		glColor3d(bl/max+0.1,bl/max+0.1,bl/max+0.1);
		glVertex3fv(v[0]);
		
		glColor3d(tl/max+0.1,tl/max+0.1,tl/max+0.1);
		glVertex3fv(v[1]);
		
		glColor3d(tr/max+0.1,tr/max+0.1,tr/max+0.1);
		glVertex3fv(v[2]);
		;
		glColor3d(br/max+0.1,br/max+0.1,br/max+0.1);
		glVertex3fv(v[3]);
		glEnd();	

		}else{
		
		glVertex3fv(v[0]);
		
		glVertex3fv(v[1]);
		
		glVertex3fv(v[2]);
		
		glVertex3fv(v[3]);
		glEnd();
		}
	//if it is flat shading, proceed, each vertex doesn't need a colour
	}else{
		if(!clearWire){
		glColor3d(center/max+0.1,center/max+0.1,center/max+0.1);
		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]);
		glVertex3fv(v[3]);
		glEnd();	

		}else{
		
		glVertex3fv(v[0]);
		
		glVertex3fv(v[1]);
		
		glVertex3fv(v[2]);
		
		glVertex3fv(v[3]);
		glEnd();
	
	}
	}	
	}
}
	glPopMatrix();//grid
}


/* createdNormalVectors - creates the normal vectors for each
vertice by getting the cross product*/
void createNormalVectors(){
	float v1[3];
	float v2[3];
	float vfinal[3];
	for(int i =0;i<sizex; i++){
		for(int j = 0;j<sizez;j++){

		//Intialize the first vertex	
		v1[0] = i+1;
		v1[1] = getHeight(i+1,j) - getHeight(i,j);
		v1[2] = j;
		
		//Intialize the second vertex
		v2[0] = i+1;
		v2[1] = getHeight(i+1,j+1) - getHeight(i,j);
		v2[2] = j+1;
		
		//perform cross product calculation
		vfinal[0] = v1[1]*v2[2] - v1[2]*v2[1];
		vfinal[1] = v1[2]*v2[0] - v1[0]*v2[2];
		vfinal[2] = v1[0]*v2[2] - v1[2]*v2[0];
		float length = sqrtf(vfinal[0]*vfinal[0] + vfinal[1]*vfinal[1] 
						+ vfinal[2]*vfinal[2]);



		}
	}


}

/*viewModeDraw() - based on the current state of the viewModel,
this function desides which mesh type to draw

The viewModeDraw() can draw either a solid terrain, a wire mesh terrain, or both
with the wire mesh being on top of the solid terrain.
*/
void viewModeDraw(){
	float origin[3] = {0,0,0};
	
	//Draw the solid terrain
	if(solid){
		glPolygonMode(GL_FRONT, GL_FILL);
		drawGrid(origin, sizex, sizez);
	}
	//Draw the wireframe terrain
	else if(wireframe){
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawGrid(origin, sizex, sizez); //draws Wireframe
	}
	//Draw both the solid and wireframe terrain
	else if(both){
		
		glPolygonMode(GL_FRONT, GL_FILL);
		drawGrid(origin, sizex, sizez); //draws floor
		clearWire = true;
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1,-1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3fv(cols[0]);
		drawGrid(origin, sizex, sizez); //draws floor
		clearWire = false;
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
	glutPostRedisplay();

}

/*keyboard() - Takes a keyboard input of various letter keys on a standard QWERTY keyboard

unsigned char key - The key that is pressed.
int x - The x coordinate of the mouse
int y - The y coordinate of the mouse
*/
void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{	glMatrixMode(GL_MODELVIEW);

		//Quit/Close/Exit the program
		case 'q':
		case 27:
			exit (0);
			break;

		//Resets the terrain
		case 'r':
		case 'R':
			max = 0;
			for(int i = 0; i<sizex; i++){
				for (int j = 0; j<sizez; j++){
				yCoords[i][j] =0;
				}
			}
			
			heightMap(sizex,sizez,rand()%(sizex/2)+10);//draws a new terrain from 10 to 20
			break;

		//Cycle through solid, wireframe, and wireframe/solid terrains
		case 'w':
		case 'W':
			
			if(solid){
			solid = !solid;
			wireframe = !wireframe;
			both = both;
			}else if(wireframe){
			solid = solid;
			wireframe = !wireframe;
			both = !both;
			}else if(both){
			solid = !solid;
			wireframe = wireframe;
			both = !both;
			}
			break;

		//Enable or disable the lighting
		case 'e':
		case 'E':
			if(lightOn){
			lightOn = !lightOn;
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
			}else{
			lightOn = !lightOn;
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
			}
			break;

		//Toggle between triangles/quads to be drawn
		case 't':
		case 'T':
			quads = !quads;
			break;

		//Change the light position with keys i, j, k, l, o, and u
		case 'i':
		case 'I':
			lightpos1[2] +=1;
			lightpos2[2] -=1;
			break;
		case 'k':
		case 'K':
			lightpos1[2] -=1;
			lightpos2[2] +=1;
			break;
		case 'j':
		case 'J':
			lightpos1[0] +=1;
			lightpos2[0] -=1;
			break;
		case 'l':
		case 'L':
			lightpos1[0] -=1;
			lightpos2[0] +=1;
			break;
		case 'o':
		case 'O':
			lightpos1[1] +=1;
			lightpos2[1] -=1;
			break;
		case 'u':
		case 'U':
			lightpos1[0] +=1;
			lightpos2[0] -=1;
			break;

		//Change to the flatview of the terrain
		case 'v':
		case 'V':
			flatview = !flatview;
			break;
		
			
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,lightpos1);
	glLightfv(GL_LIGHT1,GL_POSITION,lightpos2);
	glPopMatrix();
	glutPostRedisplay();
}
/*reset - used to reset the heightmap and clears the current mesh*/
void reset(){
	max = 0;
	for(int i = 0; i<sizex; i++){
		for (int j = 0; j<sizez; j++){
		yCoords[i][j] =0;
		}
	}
	eye[0] = 0;
	eye[1] = 5;
	eye[2] = 0;
}

/*keyboard() - Takes a keyboard input of various special keys on a standard QWERTY keyboard
These keys are keys like CTRL, SHIFT, ALT, ENTER, etc...

int key - The key that is pressed.
int x - The x coordinate of the mouse
int y - The y coordinate of the mouse
*/
void special(int key, int x, int y)
{
	/* arrow key presses move the camera and the page up and page down keys move in and out of the screen */
	switch(key){

	case GLUT_KEY_LEFT: 
		eye[0] -=1;	
	break;

	case GLUT_KEY_RIGHT: 
		eye[0] +=1;
	break;

	case GLUT_KEY_DOWN: 
		if(eye[1] != 0){
		eye[1] -=1;
		}	 
	break;

	case GLUT_KEY_UP:
		//stop moving up if it is larger than max height 
		eye[1] +=1;
	break;

	case GLUT_KEY_PAGE_UP:
		eye[2] -=1;//into the screen

	break;

	case GLUT_KEY_PAGE_DOWN:
		eye[2] +=1;//out of the screen
	break;

    }

	glutPostRedisplay();
}

/*menu() - drop down menu used to change the size of the grid by using right clicks

int key - Takes in a key to shown the menu (Right Mouse Button in this case)
*/
void menu(int key){
	
	switch(key){

		//Changes the terrain to a 50 x 50 grid
		case'1':
		sizex = 50;
		sizez = 50;
		reset();
		break;
		
		//Changes the terrain to a 100 x 100 grid
		case'2':
		sizex = 100;
		sizez = 100;
		reset();
		break;

		//Changes the terrain to a 150 x 150 grid
		case'3':
		sizex = 150;
		sizez = 150;
		reset();
		break;
		
		//Changes the terrain to a 200 x 200 grid
		case'4':
		sizex = 200;
		sizez = 200;
		reset();
		break;

		//Changes the terrain to a 300 x 300 grid
		case'5':
		sizex = 300;
		sizez = 300;
		reset();
		break;
		
	}
	glutPostRedisplay();


}

/*init(void) - Initializes the different GLUT and OpenGL methods for drawing graphics
this also enables lighting, materials, viewing perspectives, and culling.*/
void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
    	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    	glLightfv(GL_LIGHT0,GL_POSITION,lightpos1);
	glLightfv(GL_LIGHT1,GL_POSITION,lightpos2);
       	glMatrixMode(GL_PROJECTION); 
	glFrustum(-10,10,-1000,1000,0,1000);
	gluPerspective(45, 1, 1, 600); 
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	
	//***************Material setup*****************//
	float amb[] = {0.72,0.43,0.71,1.0}; //Ambient material parameters
	float diff[] = {0.28,0.70,0.12,1.0}; //Diffuse material parameters
	float spec[] = {0.98,0.90,0.79,1.0}; //Specular material parameters
	float shiny = 23; //Shininess material parameters

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb); //Set the ambient material
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff); //Set the diffuse material
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec); //Set the specular material
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny); //Set the shiny material
	glColorMaterial(GL_AMBIENT,GL_DIFFUSE);
	//************** Light setup*******************//
	GLfloat DiffuseLight1[] = {1.0,0.0,0.0};//red light
	GLfloat AmbientLight1[] = {1.0,0.0,0.0};
	GLfloat DiffuseLight2[] = {0.0,1.0,0.0};//green light
	GLfloat AmbientLight2[] = {0.0,1.0,0.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,DiffuseLight1); //Set the diffuse light for light1
	glLightfv(GL_LIGHT1,GL_DIFFUSE,DiffuseLight2); //Set the diffuse light for light2
	glLightfv(GL_LIGHT0,GL_AMBIENT,AmbientLight1); //Set the ambient light for light1
	glLightfv(GL_LIGHT1,GL_AMBIENT,AmbientLight2); //Set the ambient light for light2



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//******************** Camera Controls**********************//
	float origin[3] = {0,0,0};
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], sizex/2, 0, -sizez/2, 0, 1, 0); // have it look at the centre of mesh, z axis is flipped
	viewModeDraw();
	glutSwapBuffers();
}

/* intstructions - text that is displayed in the terminal to let
the user know how to use the program*/
void instructions(void){
	printf("Here are the instructions for the program:\n");
	printf("UP,Down  :  y camera controls		\n");
	printf("Left,Right: x camera controls 		\n");
	printf("Pg up,Pg down: z camera controls	\n");
	printf("   i,k   :  z light controls		\n");
	printf("   j,l	 :  x light controls		\n");
	printf("   u,o	 :  y light controls		\n");
	printf("   w	 :  toggle wireframe modes 	\n");
	printf("   r 	 :  Random new Terrain		\n");
	printf("   e 	 :  Toggle Lights		\n");
	printf("   v 	 :  Toggle Shader Mode		\n");
	printf("Right Click : Change size of map	\n");
	printf("   q 	 :  Quit program		\n");


}
/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Terrain");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	instructions();
	init();

	//Creates menu with different menu entries
	glutCreateMenu(menu);
	glutAddMenuEntry("50 x 50", '1');
	glutAddMenuEntry("100 x 100", '2');
	glutAddMenuEntry("150 x 150", '3');
	glutAddMenuEntry("200 x 200", '4');
	glutAddMenuEntry("300 x 300", '5');
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
