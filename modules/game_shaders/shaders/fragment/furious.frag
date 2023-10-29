uniform sampler2D u_Tex0;
varying vec2 v_TexCoord;
uniform vec2 u_Resolution; 

void main() {
    vec4 pixelColor = texture2D(u_Tex0, v_TexCoord);
    vec2 pixelSize = 1.0 / u_Resolution;
    for (float dx = -1.0; dx <= 1.0; dx++) {
        for (float dy = -1.0; dy <= 1.0; dy += 1.0) {
            vec2 offset = vec2(dx, dy) * pixelSize;
            vec4 neighborPixel = texture2D(u_Tex0, v_TexCoord + offset);
            if(neighborPixel.w != 0.0){
                if (pixelColor.w == 0.0) {
                    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Vermelho sólido
                    return;
                }
            }
        }
    }
    gl_FragColor = pixelColor;
}
