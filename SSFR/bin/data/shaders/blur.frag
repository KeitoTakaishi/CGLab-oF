#version 400
precision highp float;
uniform sampler2DRect depthTex;
uniform vec2 direction;
uniform float blurScale;
uniform float blurDepthFallOff;
in vec2 vTexCoord;

out vec4 fragColor;

void main(){
    vec2 uv = vTexCoord;
    float depth = texture(depthTex, uv ).r;
    
    float sum = 0.0;
    float wsum = 0.0;

    vec2 blurDir = vec2(direction);
    vec4 color = vec4(0.0);

   
    float N = 64;
    float skip = 1.0;
    for(float i = -N; i <= N; i+=skip){
        //uv = uv + vec2(direction.x, direction.y) * i;
        //uv.x = uv.x + direction.x * i;
        //uv.y = uv.y + direction.y * i;

        float sampleDepth = texture(depthTex, uv + vec2(direction.x, direction.y) * i).r;
        //float sampleDepth = texture(depthTex, uv).r;

        float r = blurScale * i;
        //float r = 0.4 * i;
        float w = exp(-r*r);
        
        float r2 = (sampleDepth - depth) * blurDepthFallOff;
        //float r2 = (sampleDepth - depth) * 0.1;
        float g = exp(-r2*r2);

        sum += sampleDepth * w * g;
        wsum += w * g;
    }

    if(sum > 0.0) sum /= wsum;

    
    if(depth > 1.0 || depth < 0.001){
        //fragColor = vec4(vec3(0.0), 1.0);
    }else{
        // /fragColor = vec4(vec3(sum), 1.0);
    }
    
    fragColor = vec4(vec3(sum), 1.0);
}
