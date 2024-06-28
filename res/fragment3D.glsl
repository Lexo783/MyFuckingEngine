#version 410

uniform sampler2D tex;

in vec2 uv;
out vec4 out_color;

void main()
{
    vec4 tex_color =  texture(tex, uv);
    out_color = tex_color;
}