#version 330 core

uniform vec2 mouse;
uniform bool mouseDown;
uniform int mouseButton;
uniform sampler3D guiTextures;
uniform sampler1D guiData;
out vec4 fragcolor;

void main() {
  fragcolor = vec4(gl_FragCoord.xy / 1000, 0., 0.);
}
