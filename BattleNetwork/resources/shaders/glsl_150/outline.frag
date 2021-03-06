precision lowp float;
precision lowp int;

varying vec2 vTexCoord;

uniform sampler2D texture;
uniform float edge_threshold;

void main()
{
    const float offset = 1.0 / 512.0;
    vec2 offx = vec2(offset, 0.0);
    vec2 offy = vec2(0.0, offset);

    vec4 hEdge = texture2D(texture, vTexCoord.xy - offy)        * -2.0 +
                 texture2D(texture, vTexCoord.xy + offy)        *  2.0 +
                 texture2D(texture, vTexCoord.xy - offx - offy) * -1.0 +
                 texture2D(texture, vTexCoord.xy - offx + offy) *  1.0 +
                 texture2D(texture, vTexCoord.xy + offx - offy) * -1.0 +
                 texture2D(texture, vTexCoord.xy + offx + offy) *  1.0;

    vec4 vEdge = texture2D(texture, vTexCoord.xy - offx)        *  2.0 +
                 texture2D(texture, vTexCoord.xy + offx)        * -2.0 +
                 texture2D(texture, vTexCoord.xy - offx - offy) *  1.0 +
                 texture2D(texture, vTexCoord.xy - offx + offy) * -1.0 +
                 texture2D(texture, vTexCoord.xy + offx - offy) *  1.0 +
                 texture2D(texture, vTexCoord.xy + offx + offy) * -1.0;

    vec3 result = sqrt(hEdge.rgb * hEdge.rgb + vEdge.rgb * vEdge.rgb);
    float edge = length(result);
    vec4 pixel = texture2D(texture, vTexCoord.xy);
    if (edge > (edge_threshold * 8.0))
        pixel.rgb = vec3(0.0, 0.0, 0.0);
    else
        pixel.a = edge_threshold;
    // gl_FragColor = pixel;
}