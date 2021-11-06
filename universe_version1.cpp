


// current camera position
GLfloat cameraPosition[] = { 2, 0.2, 0 };
// Point that the camera is looking at
GLfloat cameraLookAt[] = { 0, 0, 0 };


//Get the count of the faces of on the enterprise:
int enterpriseFCount = sizeof(enterpriseF) / sizeof(enterpriseF[0]);
float enterpriseTime = 0;
float xOffset = 0;
float yOffset = 0;
float scaleFactor = 0.2f;

void drawEnterprise() {
	enterpriseTime += 0.001;
	xOffset = cos((2 * M_PI) * enterpriseTime)*2;
	yOffset = sin((2 * M_PI) * enterpriseTime)*2;


	//Iterate through all the faces:
	for (int i = 0; i < enterpriseFCount; i++) {
		glBegin(GL_POLYGON);
		glColor3f((float)i/float(enterpriseFCount)+0.3, (float)i / float(enterpriseFCount)+0.3, (float)i / float(enterpriseFCount)+0.3);
		glVertex3f(		(enterpriseV[enterpriseF[i][0]][0] * scaleFactor) + xOffset,
						enterpriseV[enterpriseF[i][0]][1] * scaleFactor ,
						(enterpriseV[enterpriseF[i][0]][2] * scaleFactor) + yOffset);
		
		glVertex3f(		(enterpriseV[enterpriseF[i][1]][0] * scaleFactor) + xOffset,
						enterpriseV[enterpriseF[i][1]][1] * scaleFactor,
						(enterpriseV[enterpriseF[i][1]][2] * scaleFactor) + yOffset);

		glVertex3f(		(enterpriseV[enterpriseF[i][2]][0] * scaleFactor) + xOffset,
						enterpriseV[enterpriseF[i][2]][1] * scaleFactor,
						(enterpriseV[enterpriseF[i][2]][2] * scaleFactor) + yOffset);

		glEnd();

	}
}




//Function for keeping track of time and incrementing frames:
void timer(int) {
	glutPostRedisplay(); //Redisplay the buffer
	glutTimerFunc(1000 / 60, timer, 0); //Call the glut timerFunc on timer at a rate of 1000 times per second
}


/************************************************************************

	Function:		polygon

	Description:	Draw a polygon via list of vertices.

*************************************************************************/
void polygon(int a, int b, int c, int d)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube()
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}

//Sphere function, draws a sphere
//Radius, posX, posY, rgb color
void sphere(float radius, float x, float y, float r, float g, float b) {
	glColor3f(r,g,b);
	//Set the sphere drawing mode to be solid (GL_FILL)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Translate to the location of the sphere:
	glTranslatef(x, 0, y);
	glutSolidSphere(radius, 20, 20);
	glTranslatef(-x, 0, -y);
}

//Array that defines the planets:
//bodyRadius, orbitRadius, orbitSpeed, orbitIncrement, startingpoint, hasPlanets (true or false), r,g,b
float planets[9][9] = 
{
	//The Sun:
	{.05, 0, 0, 0, .5,		0,	1, 1, 0},

	//Small greyish planet:
	{.01, .2, 0.015, 0.007, .5,		0,	.7, .7, .7},
	//Earth planet:
	{.015, .4, 0.012, 0.005, 0,		0,	0, .7, .7},
	//Red planet:
	{.012, .6, 0.011, 0.0045, 0.1,	0,	.7, 0, 0},

	//Blue Gas Giant:
	{.035, 1.2, 0.001, 0.0003, 0.1,	0,	.7, 0, 1},
	//Orange Gas giant:
	{.024, 1.5, 0.003, 0.00025, .5,	0,	1, .5, .1},


	//Far away exoplanets:
	{.009, 2, 0.001, 0.0005, 0,		0,	.7, .7, .7},
	{.01, 2.5, 0.001, 0.0006, 0,	0,	.7, .7, .7},
};

int planetCount = sizeof(planets) / sizeof(planets[0]);

float orbitTime = 0;
void drawPlanets(){
	orbitTime += 0.001;
	
	for (int i = 0; i <= planetCount; i++)
	{
		if (planets[i][2] >= 1) {
			planets[i][2] = 0;
		}
		planets[i][2] += planets[i][3];
		sphere(planets[i][0], planets[i][1]* (cos((2 * M_PI)*planets[i][2])), planets[i][1] * (sin((2 * M_PI)*planets[i][2])), planets[i][6], planets[i][7], planets[i][8]);
	}
}

int orbit_line = 0;
void drawOrbitsCircular() {

	if (orbit_line == 0) {
		;
		for (int i = 0; i <= planetCount; i++)
		{
			glBegin(GL_LINE_LOOP);
			for (int k = 1; k < 100; k++) {
				glColor3f(1, 1, 1);
				glVertex3f(cos((2 * M_PI / 100)*k) * planets[i][1], 0, sin((2 * M_PI / 100)*k) * planets[i][1]);

			}
			glEnd();
		}
	}
}

// function to print distance
//https://www.geeksforgeeks.org/program-to-calculate-distance-between-two-points-in-3-d/
float distance(	float x1, float y1, float z1, float x2, float y2, float z2){
	float d = sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) +
		pow(z2 - z1, 2) * 1.0);
	return d;
}

GLfloat coronaEndpoints[40000][5];
int coronaDir[40000];
GLfloat stars[1000][4];

void init(void)
{

	for (int i = 0; i < 40000; ++i)
	{
		//Random corona endpoints (xyz_
		coronaEndpoints[i][0] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2)-0.1;
		coronaEndpoints[i][1] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2)-0.1;
		coronaEndpoints[i][2] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2)-0.1;
		//The corona's movement speed:
		coronaEndpoints[i][3] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.0002) - 0.0001;

		//Keep track of whether the movement is positive or negative (randomized at the start):
		coronaEndpoints[i][4] = (float)((rand() * 2) -1);
		coronaDir[i] = ((rand() * 2) - 1);

		//calculate the length of the randomized corona:
		//Check if the generate corona line is within a min/max distance constraint, regenerate if outside
		float coronaLength = distance(coronaEndpoints[i][0], coronaEndpoints[i][1], coronaEndpoints[i][2], 0, 0, 0);
		while (coronaLength <.01 ) {
			coronaEndpoints[i][0] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2) - 0.1;
			coronaEndpoints[i][1] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2) - 0.1;
			coronaEndpoints[i][2] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.2) - 0.1;
			coronaLength = distance(coronaEndpoints[i][0], coronaEndpoints[i][1], coronaEndpoints[i][2], 0, 0, 0);
		}

	}

}

int corona = 0;
void drawCorona() {
	if (corona == 0) {

		for (int i = 0; i < 40000; i++) {

			if (coronaEndpoints[i][3] > 0.08) {
		
				coronaEndpoints[i][3] = 0;
				coronaEndpoints[i][4] = -1 ;
			}

			coronaEndpoints[i][3] += (0.00000001 * coronaDir[i]);

			glBegin(GL_LINES);
				glVertex3f(0, 0, 0);
				glColor4f(1, .8, .1, 0);
				glVertex3f(coronaEndpoints[i][3]+coronaEndpoints[i][0] * ((float)rand()) / (float)RAND_MAX, coronaEndpoints[i][3] + coronaEndpoints[i][1] * ((float)rand()) / (float)RAND_MAX, coronaEndpoints[i][3]+coronaEndpoints[i][2] * ((float)rand()) / (float)RAND_MAX);
				glColor4f(1, .8, .1, 1);
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(0, 0, 0);
				glColor4f(1, .8, .1, 0);
				glVertex3f(coronaEndpoints[i][0] * ((float)rand()) / (float)RAND_MAX - coronaEndpoints[i][3],  coronaEndpoints[i][1] * ((float)rand()) / (float)RAND_MAX - coronaEndpoints[i][3], coronaEndpoints[i][2] * ((float)rand()) / (float)RAND_MAX - coronaEndpoints[i][3]);
				glColor4f(1, .8, .1, 1);
			glEnd();
		}
	}
}


//Function for handling the camera position:
void cameraFunc() {
	gluLookAt(xOffset+ cameraPosition[0], cameraPosition[1], yOffset+ cameraPosition[2], /* look from camera XYZ */
		xOffset, cameraLookAt[1], yOffset, /* look at the origin */
		0, 1, 0); /* positive Y up vector */

}


void display(void)
{
	/* display callback, clear frame buffer and z buffer,
	   rotate cube and draw, swap buffers */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glLoadIdentity();

	cameraFunc();

	// draw the color cube
	colorcube();

	drawPlanets();
	drawOrbitsCircular();

	drawEnterprise();
	drawCorona();

	glFlush();
	glutSwapBuffers();
}


void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	glutPostRedisplay();
}



void myKeys(unsigned char key, int xx, int yy)
{
	if ((key == 'r') || (key == 'R'))
		orbit_line = 1 - orbit_line;

	if ((key == 'c') || (key == 'C'))
		corona = 1 - corona;

	if ((key == 'q') || (key == 'Q')) 
		exit(0);

	glutPostRedisplay();
}

void catchKey(int key, int x, int y)
{

	if (key == GLUT_KEY_LEFT) {
		cameraPosition[2] += .1;
	}

	if (key == GLUT_KEY_RIGHT) {
		cameraPosition[2] -= .1;
	}

	if (key == GLUT_KEY_UP) {
		cameraPosition[1] += .1;
	}

	if (key == GLUT_KEY_DOWN) {
		cameraPosition[1] -= .1;
	}

	if (key == GLUT_KEY_PAGE_UP) {
		cameraPosition[0] -= .1;
	}

	if (key == GLUT_KEY_PAGE_DOWN) {
		cameraPosition[0] += .1;
	}

}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1.333, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}


void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	

	/* need both double buffering and z buffer */

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Osler Assignment 2");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	
	//Keyboard and mouse callbacks:
	glutKeyboardFunc(myKeys);
	glutMouseFunc(mouse);
	glutSpecialFunc(catchKey);


	glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
	glClearColor(0,0,0,0);
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init();
	glutTimerFunc(1000, timer, 0);

	glutMainLoop();
}
