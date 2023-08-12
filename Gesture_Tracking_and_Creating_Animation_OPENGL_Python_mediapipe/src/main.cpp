//
//  Display a rotating cube with lighting - Gouraud shading vs Phong shading
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent as vertex attributes
//

#include "Angel.h"
#include <math.h>

#define NUM_OF_FRAMES 379
#define NUM_OF_POINTS 33
#define NUM_OF_UZUV 12
#define FRAME_RATE 60

typedef vec4  color4;
typedef vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

const int NumTimesToSubdivide = 5;
const int NumTriangles        = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVerticesSphere         = 3 * NumTriangles;
vec3    tex_coords[NumVerticesSphere];
vec3    tex2_coords[NumVerticesSphere];

float first = 0; 

// Texture objects and storage for texture image
GLuint textures[4];
const int  TextureSize = 64;
GLubyte* image;
GLubyte* image2;


GLubyte image3[TextureSize][TextureSize][3];
GLubyte image4[TextureSize][TextureSize][3];

GLuint default_textures;

point4 points[NUM_OF_UZUV][NumVertices];
vec3   normals[NUM_OF_UZUV][NumVertices];
vec3 positions[NUM_OF_FRAMES][NUM_OF_POINTS];

vec3 middlePoint[NUM_OF_UZUV] = {vec3(0, 0, 0)};
GLfloat dist[NUM_OF_UZUV] = {0.0};
GLfloat thetaZ[NUM_OF_UZUV] = {0.0};
GLfloat thetaX[NUM_OF_UZUV] = {0.0};
int p1Array[NUM_OF_UZUV] = {11, 13, 12, 14, 23, 25, 24, 26, 11, 12, 24, 23};
int p2Array[NUM_OF_UZUV] = {13, 15, 14, 16, 25, 27, 26, 28, 12, 24, 23, 11};

point4 pointsSphere[NumVerticesSphere];
vec3   normalsSphere[NumVerticesSphere];

int currentFrameIdx = 0;
int displaymode = 3;
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

    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; tex2_coords[Index] = vec3( 0.0, 0.0, 0.0 ); Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[b]; tex2_coords[Index] = vec3( 0.0, 1.0, 0.0 );Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; tex2_coords[Index] = vec3( 1.0, 1.0, 0.0 );Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[a]; tex2_coords[Index] = vec3( 0.0, 0.0, 0.1 );Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[c]; tex2_coords[Index] = vec3( 1.0, 1.0, 1.0 );Index++;
    normals[cubeIndex][Index] = normal; points[cubeIndex][Index] = vertices[d]; tex2_coords[Index] = vec3( 1.0, 0.0, 0.0 );Index++;
    
    //Note that normals are fixed for a given face of the cube.
    //So the normal of a vertex is NOT computed based on neighboring faces.
    //which makes sense in this example since this is a cube with only 6 faces.
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void readPPMofBasketball() {

    FILE* fd;
    int k, n, m;
    char c;
    int i;
    char b[] = "basketball.ppm";
    int red, blue, green;

    fd = fopen(b, "r");
    fscanf(fd, "%[^\n] ", b);

    if (b[0] != 'P' || b[1] != '3') {
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c", &c);

    while (c == '#') {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }

    ungetc(c, fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);

    int nm = n * m;

    image = (GLubyte*)malloc(3 * sizeof(GLubyte) * nm);

    for (i = nm; i > 0; i--) {

        fscanf(fd, "%d %d %d", &red, &green, &blue);
        image[3 * nm - 3 * i] = red;
        image[3 * nm - 3 * i + 1] = green;
        image[3 * nm - 3 * i + 2] = blue;

    }
}

// earth.ppm read -----------------------------------------------------------------------------------------------------------------------

void readPPMofWorldMap() {

    FILE* fd;
    int k, n, m;
    char c;
    int i;
    char b[] = "giraffe_p3.ppm";
    int red, blue, green;

    fd = fopen(b, "r");
    fscanf(fd, "%[^\n] ", b);

    if (b[0] != 'P' || b[1] != '3') {
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c", &c);

    while (c == '#') {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }

    ungetc(c, fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);

    int nm = n * m;

    image2 = (GLubyte*)malloc(3 * sizeof(GLubyte) * nm);

    for (i = nm; i > 0; i--) {

        fscanf(fd, "%d %d %d", &red, &green, &blue);
        image2[3 * nm - 3 * i] = red;
        image2[3 * nm - 3 * i + 1] = green;
        image2[3 * nm - 3 * i + 2] = blue;

    }
}



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


int IndexSphere = 0;

void
triangle( const point4& a, const point4& b, const point4& c )
{
    //normal vector is computed per vertex

    vec3 norm = normalize(vec3(a.x, a.y, a.z));

    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[IndexSphere] = a;
    tex_coords[IndexSphere] = vec3(a.x, a.y, a.z); // TEXTURE POINTS for a
    IndexSphere++;
    norm = normalize(vec3(b.x, b.y, b.z));
    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[IndexSphere] = b;
    tex_coords[IndexSphere] = vec3(b.x, b.y, b.z); // TEXTURE POINTS for a
    IndexSphere++;
    norm = normalize(vec3(c.x, c.y, c.z));
    normalsSphere[IndexSphere] = vec3(norm.x, norm.y, norm.z);
    pointsSphere[IndexSphere] = c;
    tex_coords[IndexSphere] = vec3(c.x, c.y, c.z); // TEXTURE POINTS for a
    IndexSphere++;
    
}
//----------------------------------------------------------------------------

point4
unit( const point4& p )
{
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
    point4 t;
    if ( len > DivideByZeroTolerance ) {
        t = p / sqrt(len);
        t.w = 1.0;
    }
    
    return t;
}

void
divide_triangle( const point4& a, const point4& b,
                const point4& c, int count )
{
    if ( count > 0 ) {
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle(  a, v1, v2, count - 1 );
        divide_triangle(  c, v2, v3, count - 1 );
        divide_triangle(  b, v3, v1, count - 1 );
        divide_triangle( v1, v3, v2, count - 1 );
    }
    else {
        triangle( a, b, c );
    }
}

void
tetrahedron( int count )
{
    point4 v[4] = {
        vec4( 0.0, 0.0, 1.0, 1.0 ),
        vec4( 0.0, 0.942809, -0.333333, 1.0 ),
        vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
        vec4( 0.816497, -0.471405, -0.333333, 1.0 )
    };
    
    divide_triangle( v[0], v[1], v[2], count );
    divide_triangle( v[3], v[2], v[1], count );
    divide_triangle( v[0], v[3], v[1], count );
    divide_triangle( v[0], v[2], v[3], count );
}

//----------------------------------------------------------------------------
GLuint vao[NUM_OF_UZUV+1];
GLuint buffer[NUM_OF_UZUV+1];


// OpenGL initialization
void
init(GLFWwindow* window)
{
    glewExperimental = GL_TRUE; 
    glewInit();
    colorcube();
    tetrahedron(NumTimesToSubdivide);

        // Create a checkerboard pattern
    for ( int i = 0; i < 64; i++ ) {
        for ( int j = 0; j < 64; j++ ) {
            GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
            image3[i][j][0]  = c;
            image3[i][j][1]  = c;
            image3[i][j][2]  = c;
            image4[i][j][0] = 0; // you can change the color of the second texture pattern as you like
            image4[i][j][1] = c;
            image4[i][j][2] = c;
        }
    }

    

    // Open landmarks file
    FILE* positionsFile = fopen("landmarks.txt", "r");
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
        positions[frameIdx][locIdx] = vec3((x-640.0)/360.0, (360.0-y)/360.0, z);
        //positions[frameIdx][locIdx] = vec3(x, y, z);
        i++;
    }
    printf("%d\n",frameIdx);
    fclose(positionsFile);





    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" ); //use vshader_phong.glsl, fshader_phong.glsl for phong shading
    glUseProgram( program );

    // Create a vertex array object
    glGenVertexArrays( NUM_OF_UZUV+1, vao );

    
    glGenTextures(4, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 413, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);




        glBindTexture( GL_TEXTURE_2D, textures[2] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
		  GL_RGB, GL_UNSIGNED_BYTE, image3 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //try here different alternatives
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //try here different alternatives

    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
		  GL_RGB, GL_UNSIGNED_BYTE, image4 );
    //glGenerateMipmap(GL_TEXTURE_2D); // try also activating mipmaps for the second texture object
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    glBindTexture( GL_TEXTURE_2D, textures[1] ); //set current texture

   

    
    
    for(int i = 0; i < NUM_OF_UZUV; i++) {
        glBindVertexArray( vao[i] );
        
        // Create and initialize a buffer object
        
        glGenBuffers( 1, &buffer[i] );
        glBindBuffer( GL_ARRAY_BUFFER, buffer[i] );
        glBufferData( GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(normals[i]), NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points[i]), points[i] );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(points[i]), sizeof(normals[i]), normals[i] );
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere), sizeof(tex2_coords), tex2_coords);
        
        // set up vertex arrays
        GLuint vPosition = glGetAttribLocation( program, "vPosition" );
        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        
        GLuint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])) );

        GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                BUFFER_OFFSET(sizeof(points[i])+sizeof(normals[i]))  );
        
    }

    
    glBindVertexArray( vao[NUM_OF_UZUV] );
    
    // Create and initialize a buffer object
    
    // glGenBuffers( 1, &buffer[NUM_OF_UZUV] );
    // glBindBuffer( GL_ARRAY_BUFFER, buffer[NUM_OF_UZUV] );
    // glBufferData( GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere), NULL, GL_STATIC_DRAW );
    // glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(pointsSphere), pointsSphere );
    // glBufferSubData( GL_ARRAY_BUFFER, sizeof(pointsSphere), sizeof(normalsSphere), normalsSphere );
    
    
    
    // // set up vertex arrays
    // GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    // glEnableVertexAttribArray( vPosition );
    // glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    // GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    // glEnableVertexAttribArray( vNormal );
    // glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])) );

    glBindTexture(GL_TEXTURE_2D, textures[1]);


    glGenBuffers( 1, &buffer[NUM_OF_UZUV] );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[NUM_OF_UZUV] );
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere) + sizeof(tex_coords),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsSphere), pointsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere), sizeof(normalsSphere), normalsSphere);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSphere) + sizeof(normalsSphere), sizeof(tex_coords), tex_coords);


    // Set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //GLuint vColor = glGetAttribLocation(program, "vColor");
    //glEnableVertexAttribArray(vColor);
    //glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere)));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere)));

    //GLuint vCoords = glGetAttribLocation(program, "vCoords");
    //glEnableVertexAttribArray(vCoords);
    //glVertexAttribPointer(vCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4) * 3 * NumTrianglesSphere + sizeof(vec3) * 3 * NumTrianglesSphere));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSphere) + sizeof(normalsSphere)));

    // Load shaders and use the resulting shader program
    

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

    //glUniform1i(glGetUniformLocation(program, "ShaderFlag"), 1);
    glUniform1i(glGetUniformLocation(program, "TextureFlag"), 1);
   
    
	
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
                Scale(dist[i] * 0.1, dist[i], dist[i] * 0.1));
        
        glBindVertexArray( vao[i] );
        glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

        glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    }

    
    mat4  model_view = ( Translate( positions[currentFrameIdx-1][0] ) *
                Scale(0.1, 0.1, 0.1));

        
    glBindVertexArray( vao[NUM_OF_UZUV] );
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    


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
    
    //currentFrameIdx = 1;
    //currentFrameIdx++;
    currentFrameIdx = (currentFrameIdx + 1) % (NUM_OF_FRAMES);
    //printf("Current frame: %d\n",currentFrameIdx);
}

//----------------------------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        if (action == GLFW_RELEASE) {
        return;
    }

    switch( key ) {
        case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
            exit( EXIT_SUCCESS );
            break;

        case GLFW_KEY_T:
        displaymode -= 1;
        if (displaymode == 2) {

            glBindTexture(GL_TEXTURE_2D, textures[0]);

        }
        else if (displaymode == 1) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            displaymode = 3;
        }
        break;


        case GLFW_KEY_S:

        break;
    }
     
}

// Reshape the application window, preserving the object aspect ratio
// Screen reshape when the size changed code taken from lecture notes
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

    
    readPPMofBasketball();
    readPPMofWorldMap();


    
    if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    init(window);

    double frameRate = 30, currentTime, previousTime = 0.0;
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
