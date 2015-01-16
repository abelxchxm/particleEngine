uniform vec4 Diffuse;
uniform vec4 Ambient;

uniform sampler2D cloudTexture;
varying vec2 Texcoord;

varying vec3 LightDirection;
varying vec3 LightDirection2;
varying vec3 Normal;

void main(void)
{
    float facingRatio = dot(normalize(Normal), normalize(LightDirection));

    vec4 diffuseColor = texture2D(cloudTexture, Texcoord);

    vec4 light1Color = diffuseColor * Diffuse * facingRatio;

    gl_FragColor = light1Color + Ambient * diffuseColor;
}
