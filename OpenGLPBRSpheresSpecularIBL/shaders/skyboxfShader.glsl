

#version 430 core

in vec3 localPos;

uniform samplerCube skyboxMap;

out vec4 fragColor;

void main()
{		 
    vec3 color = texture(skyboxMap, localPos).rgb;
    color = color/(color + vec3(1.0)); //Simple HDR tonemapping
    color = pow(color, vec3(1.0/2.2)); //Gamma correction


    fragColor = vec4(color, 1.0);
}

