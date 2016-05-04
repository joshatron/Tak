#version 330

in vec3 colorIn;

out vec4 color_out;

void main() {
  color_out = vec4(colorIn, 1.f);
}
