#version 400 core
in vec4 vs_color;
out vec4 fs_color;
void main(){
    if(vs_color.a < 0.5f){
		discard;
	}
	fs_color=vs_color;
}