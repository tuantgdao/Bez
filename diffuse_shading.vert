// Vertex shader for per-pixel diffuse shading.The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the unit light direction and unit
// surface normal, which are passed to the fragment
// shader as varying variables.

varying vec4 diffuse,ambientGlobal,ambient, ecPos;
varying vec3 normal,halfVector;
 
void main()
{   
    vec3 aux;
     
    /* first transform the normal into eye space and normalize the result */
    normal = normalize(gl_NormalMatrix * gl_Normal);
 
    /* compute the vertex position  in camera space. */
    ecPos = gl_ModelViewMatrix * gl_Vertex;
 
    /* Normalize the halfVector to pass it to the fragment shader */
    halfVector = gl_LightSource[1].halfVector.xyz;
     
    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[1].ambient;
    ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
     
         
    gl_Position = ftransform();
} 