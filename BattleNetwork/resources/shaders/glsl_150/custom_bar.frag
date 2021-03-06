precision lowp float;
precision lowp int;

varying vec4 vColor;
varying vec2 vTexCoord;
uniform sampler2D texture;
uniform float factor;

void main()
{
    vec2 pos = vTexCoord.xy;
    vec4 pixel = texture2D(texture, pos.xy);

    float redStart = 127.0;
    float redEnd = 255.0;
    float value = clamp(factor, 0.0, 1.0);
    float delta = mix(redStart, redEnd, value);
    delta = delta/redEnd;

    // Target the red gradient
    if(pixel.r >= (redStart/redEnd) && pixel.g == 0.0 && pixel.b == 0.0) 
    {
      float prevR = pixel.r;
      pixel.r = 0.0;

      // If the timer is complete...
      if(factor >= 1.0)
      {
        pixel.g = sin((pos.x+(factor*2.0))*100.0) + 0.5*sin(pos.x+(factor)*100.0);
      }
      else if(delta > prevR) // Otherwise change the gradient to a loading bar
      {
        pixel.r = 224.0/255.0;
        pixel.g = 232.0/255.0;
        pixel.b = 248.0/255.0;
      }
    }

    gl_FragColor = pixel;
}