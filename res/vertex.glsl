#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_normal;
uniform mat4 view_projection_matrix;
out vec2 uv;


void main()
{
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
    uv = in_UV;
}


/**
2D
void main()
{

uniform vec2 moveCube; // Vous pouvez mettre le type que vous voulez, et le nom que vous voulez
uniform float aspect_ratio = 1;
uniform bool fullScreen = true;

    vec2 position = in_position;
    if(!fullScreen){
        position += moveCube;
        gl_Position = vec4(position.x / aspect_ratio, position.y, 0., 1.);
        return;
    }
    gl_Position = vec4(position, 0., 1.);

}
*/