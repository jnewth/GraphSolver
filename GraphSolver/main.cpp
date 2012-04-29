
#include <string.h>
#include <math.h>
#include <opengl.h>
#include <gl.h>
#include <glut.h>
#include "SDL.h"
#include <string>
#include <sstream>
#include <iostream>

#include "GridGraph.h"
#include "GridFileReader.h"
#include "GridGraphRenderer.h"
#include "GridGraphSolver.h"
#include "DFSGridGraphSolver.h"
#include "BFSGridGraphSolver.h"
#include "AStarGridGraphSolver.h"
#include "MyBFSGridGraphSolver.h"


static SDL_Surface *gScreen;

using namespace std;

//solver statics
static GridGraph *gGraph;
static GridGraphRenderer *gRenderer;
static GridGraphSolver *gSolver;

//text draw info
static int gRasterTextPosX = 570;
static int gRasterTextPosY = 10;

static const char *gSolverStringDescription;

//animation
static const int STEP_FRAME_COUNT = 30;
static bool gDone;

//animation modes
typedef enum {STEP = 0, ANIMATE, SOLVE};
static int gAnimationMode = STEP;

//solver modes
typedef enum {DFS = 0, BFS, ASTAR, ALTBFS};

//button input for stepping
static bool gStep = false;

static void tidyUpAndExit() {
	if (gGraph) {
		delete gGraph;
	}
	if (gRenderer) {
		delete gRenderer;
	}
	if (gSolver) {
		delete gSolver;
	}
	SDL_Quit();
	exit(0);

}

static void initAttributes ()
{
    // Setup attributes we want for the OpenGL context
    
    int value;
    
    // Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
    //    Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
    //    5-5-5 RGB for 16-bit screens
    
    // Request a 16-bit depth buffer (without this, there is no depth buffer)
    value = 16;
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);
    
    
    // Request double-buffered OpenGL
    //     The fact that windows are double-buffered on Mac OS X has no effect
    //     on OpenGL double buffering.
    value = 1;
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, value);
}


static void createSurface (int fullscreen)
{
    Uint32 flags = 0;
    
    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;
    
    // Create window
    gScreen = SDL_SetVideoMode (640, 480, 0, flags);
    if (gScreen == NULL) {
        fprintf (stderr, "Couldn't set 640x480 OpenGL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

}

static void initWindow() {

	//set up 2D Perspective
	SDL_WM_SetCaption("GraphSolver", "GraphSolver");
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION); //prevents Z from affecting anything.
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

}

static void setSolver(GridGraphSolverCInfoT solverInfo, int solverType) {
	switch (solverType) {
	case DFS:
		gSolver = new DFSGridGraphSolver(solverInfo);
		gSolverStringDescription = "DFS";
		break;
	case BFS:
		gSolver = new BFSGridGraphSolver(solverInfo);
		gSolverStringDescription = "BFS";
		break;
	case ASTAR:
		gSolver = new AStarGridGraphSolver(solverInfo);
		gSolverStringDescription = "A*";
		break;
	case ALTBFS:
		gSolver = new MyBFSGridGraphSolver(solverInfo);
		gSolverStringDescription = "ALTBFS";
		break;
	default:
		gSolver = NULL;
		break;
	}
}

static void init(int argc, char *argv[])
{
	if (argc <= 1) {
		cout << "Usage: GraphSolver [maze] [solver] [animation] [starty] [startx] [finishy] [finishx]" << endl;
		cout << "[maze] = 0-" << (GridFileReader::NUM_MAZES-1) << " (different mazes)" << endl;
		cout << "[solver] = 0=dfs, 1=bfs, 2=astar, 3=altbfs" << endl;
		cout << "[animation] = 0=step (press 's' to advance), 1 = animate, 2=solve";
		cout << "x and y fields specify where in the maze to start and end" << endl;
		tidyUpAndExit();
	}
	// Init SDL video subsystem
	if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {

        fprintf(stderr, "Couldn't initialize SDL: %s\n",
			SDL_GetError());
		exit(1);
	}
    // Set GL context attributes
    initAttributes();

    // Create GL context
    createSurface(0);

	initWindow();

	cout << "Maze type is " << argv[1] << endl;;
	gGraph = GridFileReader::getMaze(atoi(argv[1])); //will be null if bad number is passed in
	std::cout << "Graph is:" << std::endl << *gGraph << std::endl;

	int solverType = atoi(argv[2]);
	cout << "Solver type is " << argv[2] << endl;
	gAnimationMode = atoi(argv[3]);

	//create objects
	GridGraphSolverCInfoT solverInfo;
	solverInfo.graph = gGraph;
	cout << "To: " << argv[4] << ", " << argv[5] << endl;
 	solverInfo.from = &gGraph->getNode(atoi(argv[4]), atoi(argv[5]));
	cout << "From: " << argv[6] << ", " << argv[7] << endl;

	solverInfo.to = &gGraph->getNode(atoi(argv[6]), atoi(argv[7]));

	//graphics settings
	solverInfo.width = 550; //tell renderer its working area
	solverInfo.height = 380;
	solverInfo.xOrigin = 10; //and tell it where it's upper left corner is
	solverInfo.yOrigin = 10;

	setSolver(solverInfo, solverType);

	GridGraphRenderer::GridGraphRendererCInfoT renderInfo;
	renderInfo.graph = gGraph;
	renderInfo.width = solverInfo.width; //tell renderer its working area
	renderInfo.height = solverInfo.height;
	renderInfo.xOrigin = solverInfo.xOrigin; //and tell it where it's upper left corner is
	renderInfo.yOrigin = solverInfo.yOrigin;
	gRenderer = new GridGraphRenderer(renderInfo);
}

static void update() {
	static int updateCounter = STEP_FRAME_COUNT;

	switch (gAnimationMode) {
	case SOLVE:
		gSolver->solve();
		break;
	case ANIMATE:
		if (!updateCounter--) {
			gSolver->step();
			updateCounter = STEP_FRAME_COUNT;
		}
		break;
	case STEP:
		if (gStep) {
			gSolver->step();
			gStep = false;
		}
		break;
	}
}

static void drawStats() {
	int lineDelta = 15;
	const char *cStr = gSolverStringDescription;
	glRasterPos2f(gRasterTextPosX, gRasterTextPosY);
	while(*cStr) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cStr++);
	}

	glRasterPos2f(gRasterTextPosX, gRasterTextPosY + lineDelta);
	std::stringstream count;
	count <<  "Steps: " << gSolver->getStepCount();
	std::string temp = count.str();
	cStr = temp.c_str();
	while(*cStr) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cStr++);
	}
}

static void drawGL ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gRenderer->render();
	gSolver->render();
	drawStats();

	SDL_GL_SwapBuffers();
}

static void handleKey(SDLKey key) {
	if (key == SDLK_ESCAPE) {
		gDone = true;
	}
	if (key == SDLK_s) {
		gStep = true;
	}
	if (key == SDLK_t) {
		gAnimationMode = SOLVE;
	}
}

static void mainLoop ()
{
	gDone = false;
    SDL_Event event;
    int fps = 60;
	int delay = 1000/fps;
    int thenTicks = -1;
    int nowTicks;
    
	//We need to write a real file parser to define graphs and then pass them in to our solver
	std::string fname = "null";
    

    while ( !gDone ) {
        
		/* Check for events */
		while ( SDL_PollEvent (&event) ) {
			switch (event.type) {
                    
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					/* Any keypress quits the app... */
					handleKey(event.key.keysym.sym);
					break;
				case SDL_QUIT:
					gDone = true;
					break;
				default:
					break;
			}
		}
        
        //     This approach is not normally recommended - it is better to
        //     use time-based animation and run as fast as possible
		update();
        drawGL();
        SDL_GL_SwapBuffers();
        
        // Time how long each draw-swap-delay cycle takes
        // and adjust delay to get closer to target framerate
        if (thenTicks > 0) {
            nowTicks = SDL_GetTicks ();
            delay += (1000/fps - (nowTicks-thenTicks));
            thenTicks = nowTicks;
            if (delay < 0)
                delay = 1000/fps;
        }
        else {
            thenTicks = SDL_GetTicks ();
        }
        
        SDL_Delay (delay);
	}
}


//add command line args:
//1 maze
//2 solver
//3 step, solve, animate
int main(int argc, char *argv[])
{

	//run some unit tests
	GridNode::test();
	GridGraph::test();
    
	//sets up everything
    init(argc, argv);
    
    // Draw, get events...
    mainLoop();
    
    // Cleanup
    tidyUpAndExit();

    return 0;
}
