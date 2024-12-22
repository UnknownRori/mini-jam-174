precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;

void main() {
    // Fetch the color of the texture
    vec4 texel = texture2D(texture0, fragTexCoord);
    
    // Render the texture as white, keeping the alpha
    gl_FragColor = vec4(1.0, 1.0, 1.0, texel.a);
}
