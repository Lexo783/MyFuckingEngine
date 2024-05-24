#version 410

out vec4 out_color;
uniform vec4 color;
in vec3 vertex_position;
in vec2 uv;
uniform sampler2D m_texture;

void main()
{
    vec4 texture_color = texture(m_texture, uv);
    out_color = texture_color;
}