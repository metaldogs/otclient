attribute vec2 a_TexCoord;
attribute vec2 a_Vertex;
uniform mat3 u_TextureMatrix;
uniform mat3 u_TransformMatrix;
uniform mat3 u_ProjectionMatrix;
varying vec2 v_TexCoord;
uniform vec2 u_Offset = vec2(30.0,0.0);

highp vec4 calculatePosition() {

    return vec4(u_ProjectionMatrix * u_TransformMatrix * vec3(a_Vertex.xy + u_Offset, 1.0), 1.0);
}

void main()
{
    gl_Position = calculatePosition();
    v_TexCoord = (u_TextureMatrix * vec3(a_TexCoord,1.0)).xy;
}
