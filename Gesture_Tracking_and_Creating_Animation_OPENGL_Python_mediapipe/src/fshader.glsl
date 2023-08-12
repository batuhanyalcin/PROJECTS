#version 410

in  vec4 color;
in  vec3 texCoord;
in  vec3 fN;
in  vec3 fL;
in  vec3 fV;
uniform int TextureFlag;
out vec4 fcolor;


uniform sampler2D tex_coords;
uniform sampler2D tex2_coords;
uniform int ShaderFlag;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

void main()
{
    vec2 longitudeLatitude = vec2((atan(texCoord.y, texCoord.x) / 3.1415926 + 1.0) * 0.5,
        (asin(texCoord.z) / 3.1415926 + 0.5));
    if (ShaderFlag == 0) {
         // TAKEN FROM INTERNET CHECK REFERANCES...
        
            //sample a texture color from texture object
        if (TextureFlag == 1) {
            fcolor = texture(tex_coords, longitudeLatitude) * color;
        }
        else {
            fcolor = color;
        }
    }
    else {
        // Normalize the input lighting vectors
        vec3 N = normalize(fN);
        vec3 V = normalize(fV);
        vec3 L = normalize(fL);

        vec3 H = normalize( L + V );
        
        vec4 ambient = AmbientProduct;

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
        
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        if (TextureFlag == 1) {
            fcolor = (ambient + diffuse + specular)* texture(tex2_coords, longitudeLatitude);
        }
        else {
            fcolor = ambient + diffuse + specular;
        }

        fcolor = (ambient + diffuse + specular)* texture(tex2_coords, longitudeLatitude);
        fcolor.a = 1.0;

    }
    

    
}