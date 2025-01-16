#version 400 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec4 color;
layout(location = 3) in float jiggle_coefficient;
layout(location = 4) in float damping_coefficient;

out vec4 vs_color;

uniform mat4 camera;
uniform mat4 model;
uniform mat4 normalMatrix;

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform float ambient;

uniform float time;
uniform vec3 last_movement_normal;
uniform float last_movement_time;// if equal to time, then object is moving
uniform float first_movement_time;

void main(){
    gl_Position = vec4(vec3(model * vec4(pos, 1.0f)), 1.0);
    if(jiggle_coefficient > 0 && damping_coefficient > 0){
        if(last_movement_time == time){ 
            // if object is moving, then fake inertia
            float t=time-first_movement_time;
            if(t >  1.57){ // 90 degree in radians
                gl_Position += vec4(-last_movement_normal * jiggle_coefficient, 0.0);
            }
            else{ // when moving started, vertex should come to the inertia point in time
                gl_Position += vec4(-last_movement_normal * jiggle_coefficient * sin(t), 0.0);
            }
        }
        else{
            // if object isn't moving, then make a pendulum movement 
            // then make vertex come to the point where it should be
            float t=time-last_movement_time;
            float damping=exp(-damping_coefficient * t);
            damping = damping>0.05?damping:0;
            if(damping!=0){
                gl_Position +=vec4(sin(t+90) * jiggle_coefficient * -last_movement_normal * damping, 0.0);
            }
        }
    }
    gl_Position = camera * gl_Position;
    vec3 normal = normalize(norm * mat3(normalMatrix));
    vec3 lightDirection = normalize(lightDir);
	float diffuse = max(dot(normal, lightDirection), 0.0);
	vs_color = color*vec4(lightColor*(diffuse + ambient),1.0);
}