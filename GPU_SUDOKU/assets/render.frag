#version 150 core
#extension all : warn

in float value;

out vec4 color;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void)
{
	if( value != 0.0 ) {
		float normalized = value / 9.0;
		color = vec4(hsv2rgb(vec3(normalized, 1.0, 1.0)), 1.0);
	}
	else {
		color = vec4(0.0);
	}
}