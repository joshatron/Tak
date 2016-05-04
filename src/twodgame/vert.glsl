#version 330

in vec2 position;
in vec3 color;

uniform mat4 projection;

out vec3 colorIn;

void main() {
  gl_Position = projection * vec4(position.x, position.y, 0, 1);
  colorIn = color;
}
