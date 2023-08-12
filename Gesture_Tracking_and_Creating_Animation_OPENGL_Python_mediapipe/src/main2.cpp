//
//  Display a rotating cube with lighting - Gouraud shading vs Phong shading
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent as vertex attributes
//

#include "Angel.h"
#include <math.h>

#define NUM_OF_FRAMES 258
#define NUM_OF_POINTS 21
#define NUM_OF_UZUV 21
#define FRAME_RATE 60

typedef vec4  color4;
typedef vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)


point4 points[NUM_OF_UZUV][NumVertices];
vec3   normals[NUM_OF_UZUV][NumVertices];
vec3 positions[NUM_OF_FRAMES][NUM_OF_POINTS];

vec3 middlePoint[NUM_OF_UZUV] = {vec3(0, 0, 0)};
GLfloat dist[NUM_OF_UZUV] = {0.0};
GLfloat thetaZ[NUM_OF_UZUV] = {0.0};
GLfloat thetaX[NUM_OF_UZUV] = {0.0};
int p1Array[NUM_OF_UZUV] = {0, 1, 2, 3, 0, 5, 9, 13, 17, 5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18, 19};
int p2Array[NUM_OF_UZUV] = {1, 2, 3, 4, 5, 9, 13, 17, 0, 6, 7, 8, 10, 11, 12, 14, 15, 16, 18, 19, 20};


int currentFrameIdx = 0;
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void
quad( int a, int b, int c, int d, int cubeIndex )
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 normal = normalize( cross(u, v) );

    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[b]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[d]; Index++;
    
    //Note that normals are fixed for a given face of the cube.
    //So the normal of a vertex is NOT computed based on neighboring faces.
    //which makes sense in this example since this is a cube with only 6 faces.
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    for(int i = 0; i < NUM_OF_UZUV; i++){
        quad( 1, 0, 3, 2, i );
        quad( 2, 3, 7, 6, i );
        quad( 3, 0, 4, 7, i );
        quad( 6, 5, 1, 2, i );
        quad( 4, 5, 6, 7, i );
        quad( 5, 4, 0, 1, i );
        Index = 0;
    }

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//----------------------------------------------------------------------------
GLuint vao[NUM_OF_UZUV];
GLuint buffer[NUM_OF_UZUV];


// OpenGL initialization
void
init(GLFWwindow* window)
{
    glewExperimental = GL_TRUE; 
    glewInit();
    colorcube();
    

    // Open landmarks file
    FILE* positionsFile = fopen("landmarks_hand.txt", "r");
    if (positionsFile == NULL) {
        printf("File cannot be opened\n");
    }

    // Read file content and save it to positions
    float x, y, z;
    int frameIdx;
    int locIdx;
    int i = 0;
    while (fscanf(positionsFile, "%f %f %f", &x, &y, &z) == 3) {
        locIdx = i % NUM_OF_POINTS;
        frameIdx = i / NUM_OF_POINTS;
        //positions[frameIdx][locIdx] = vec3((x-640.0)/640.0, (y-360.0)/640.0, z);
        positions[frameIdx][locIdx] = vec3((x-240.0)/424.0, (424.0-y)/424.0, z);
        //positions[frameIdx][locIdx] = vec3(x, y, z);
        i++;
    }
    printf("%d\n",frameIdx);
    fclose(positionsFile);

    GLuint program = InitShader( "vshader_phong.glsl", "fshader_phong.glsl" ); //use vshader_phong.glsl, fshader_phong.glsl for phong shading
    glUseProgram( program );

    // Create a vertex array object
    glGenVertexArrays( NUM_OF_UZUV, vao );

    
    
    for(int i = 0; i < NUM_OF_UZUV; i++) {
        glBindVertexArray( vao[i] );
        
        // Create and initialize a buffer object
        
        glGenBuffers( 1, &buffer[i] );
        glBindBuffer( GL_ARRAY_BUFFER, buffer[i] );
        glBufferData( GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(normals[i]), NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points[i]), points[i] );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(points[i]), sizeof(normals[i]), normals[i] );
        
        // set up vertex arrays
        GLuint vPosition = glGetAttribLocation( program, "vPosition" );
        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        
        GLuint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])) );
        
    }
    
    // set up vertex arrays


    // Initialize shader lighting parameters
    point4 light_position( 0.0, 0.0, 1.0, 0.0 ); //directional light source
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 ); // L_a
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 ); // L_d
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 ); // L_s

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 ); // k_a
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 ); // k_d
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 ); // k_s
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
    color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
    color4 specular_product = light_specular * material_specular; // k_s * L_s
    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
	
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );
    
	
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    mat4  projection;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // get current window size
    //displacement = vec3(0, 0, 2.5);
    //GLfloat aspect = GLfloat(width) / height;
    //projection = Perspective(45, aspect, Zclose, Zaway);
    
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);
    
    //projection = Perspective( 60.0, -1.0, -2.0, 6.0 );
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    glEnable( GL_DEPTH_TEST );
        // Enabling culling edge
    glEnable(GL_CULL_FACE);
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 

}

//----------------------------------------------------------------------------


void
display( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    const vec3 viewer_pos( 0.0, 0.0, 6.0 );
    
    for(int i = 0; i < NUM_OF_UZUV; i++){
        mat4  model_view = ( Translate( middlePoint[i] ) *
                RotateZ( thetaZ[i] ) *
                RotateX( thetaX[i] ) *
                Scale(dist[i] * 0.2, dist[i], dist[i] * 0.2));
        
        glBindVertexArray( vao[i] );
        glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

        glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    }
    


    glFlush();
}

//----------------------------------------------------------------------------

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if ( action == GLFW_PRESS ) {
        switch( button ) {
            case GLFW_MOUSE_BUTTON_RIGHT:    Axis = Xaxis;  break;
            case GLFW_MOUSE_BUTTON_MIDDLE:  Axis = Yaxis;  break;
            case GLFW_MOUSE_BUTTON_LEFT:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------
void updateUzuv(int p1, int p2, int index){
    middlePoint[index] = (positions[currentFrameIdx][p1] + positions[currentFrameIdx][p2]) / 2;
    dist[index] = sqrt(dot((positions[currentFrameIdx][p1] - positions[currentFrameIdx][p2]), (positions[currentFrameIdx][p1] - positions[currentFrameIdx][p2])));
    thetaZ[index] = - atan(((positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x) /  (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y))) * 180 / 3.14159265;
    thetaX[index] = atan((positions[currentFrameIdx][p1].z - positions[currentFrameIdx][p2].z) / sqrt(((positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x) * (positions[currentFrameIdx][p1].x - positions[currentFrameIdx][p2].x)  + (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y) * (positions[currentFrameIdx][p1].y - positions[currentFrameIdx][p2].y)))) * 180 / 3.14159265;
    
    
}


void update( void )
{

    for (int i = 0; i < NUM_OF_UZUV; i++) {
        updateUzuv(p1Array[i], p2Array[i], i);
    }
    /*
    middlePoint[0] = (positions[currentFrameIdx][13] + positions[currentFrameIdx][11]) / 2;
    dist[0] = sqrt(dot((positions[currentFrameIdx][13] - positions[currentFrameIdx][11]), (positions[currentFrameIdx][13] - positions[currentFrameIdx][11])));
    thetaZ[0] = - atan(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x) /  (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y))) * 180 / 3.14159265;
    thetaX[0] = atan((positions[currentFrameIdx][13].z - positions[currentFrameIdx][11].z) / sqrt(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x) * (positions[currentFrameIdx][13].x - positions[currentFrameIdx][11].x)  + (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y) * (positions[currentFrameIdx][13].y - positions[currentFrameIdx][11].y)))) * 180 / 3.14159265;
    
    middlePoint[1] = (positions[currentFrameIdx][13] + positions[currentFrameIdx][15]) / 2;
    dist[1] = sqrt(dot((positions[currentFrameIdx][13] - positions[currentFrameIdx][15]), (positions[currentFrameIdx][13] - positions[currentFrameIdx][15])));
    thetaZ[1] = - atan(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x) /  (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y))) * 180 / 3.14159265;
    thetaX[1] = atan((positions[currentFrameIdx][13].z - positions[currentFrameIdx][15].z) / sqrt(((positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x) * (positions[currentFrameIdx][13].x - positions[currentFrameIdx][15].x)  + (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y) * (positions[currentFrameIdx][13].y - positions[currentFrameIdx][15].y)))) * 180 / 3.14159265;
    */
    
    //currentFrameIdx++;
    currentFrameIdx = (currentFrameIdx + 1) % (NUM_OF_FRAMES);
    //printf("Current frame: %d\n",currentFrameIdx);
}

//----------------------------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch( key ) {
        case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
            exit( EXIT_SUCCESS );
            break;
    }
}


//----------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //may not need this since the default is usually the window size

    // Set projection matrix
    mat4  projection;
    if (width <= height)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    else
        projection = Ortho(-1.0 * (GLfloat)width / (GLfloat)height, 1.0 *
            (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
    glfwPollEvents();
    glfwSwapBuffers(window);
}
//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    if (!glfwInit())
            exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(1024, 1024, "Shading", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    init(window);

    double frameRate = 60, currentTime, previousTime = 0.0;
    update();
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1/frameRate){
            previousTime = currentTime;
            update();
        }
        
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
