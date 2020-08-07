#version 410
uniform sampler2DRect tex;
uniform vec2 size;
in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    outputColor = texture(tex, texCoordVarying * size);
    //outputColor = vec4(n/3.0, 0.0, 0.0, 1.0);
}
