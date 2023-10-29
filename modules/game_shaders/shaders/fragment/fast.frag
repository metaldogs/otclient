uniform sampler2D u_Tex0;
varying vec2 v_TexCoord;
uniform lowp vec4 u_Color;

void main() {
    vec4 pixelColor = texture2D(u_Tex0, v_TexCoord); 
    gl_FragColor = pixelColor * u_Color;
}
