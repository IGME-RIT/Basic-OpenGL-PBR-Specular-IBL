

#version 430 core

in vec3 localPos;

uniform sampler2D equirectangularMap;

out vec4 fragColor;

//vec2(1/2PI, 1/PI)
const vec2 invAtan = vec2(0.1591, 0.3183);

//Lets us sample from a equirectangular Map as if it was a cubemap
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPos)); 
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    fragColor = vec4(color, 1.0);
}

