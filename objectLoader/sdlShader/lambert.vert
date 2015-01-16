varying vec2 Texcoord;

varying vec3 Normal;
varying vec3 LightDirection;
uniform vec3 fvLightPosition;

void main(void)
{
    gl_Position = ftransform();

    Texcoord = gl_MultiTexCoord0.xy;

    vec4 objectPosition = gl_ModelViewMatrix * gl_Vertex;

    LightDirection = (gl_ModelViewMatrix * vec4(fvLightPosition, 1)).xyz - objectPosition.xyz;
    Normal = gl_NormalMatrix * gl_Normal;
}
