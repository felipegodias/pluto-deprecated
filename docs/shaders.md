# Shader Definitions
All shader definitions must be used with the `#define` preprocessor inside `.glsl` files.
## Blending
`PLUTO_BLEND SRC_FACTOR DST_FACTOR`: Configure and enable blending. The generated color is multiplied by the `SRC_FACTOR`. The color already on screen is multiplied by `DST_FACTOR` and the two are added together.

`PLUTO_BLEND SRC_FACTOR DST_FACTOR SRC_FACTOR_A DST_FACTOR_A`: Same as above, but use different factors for blending the alpha channel.

`PLUTO_BLEND_EQUATION OFF`: Turn off blending (this is the default).

`PLUTO_BLEND_EQUATION EQUATION`: Turn on blending with the choosen equation.

`PLUTO_BLEND_EQUATION EQUATION EQUATION_A`: Same as above, but use different blend operation for color (RGB) and alpha (A) channels.
### Factors
|Factor|Description|
|--|--|
|ZERO|The value of one - use this to let either the source or the destination color come through fully.|
|ONE|The value zero - use this to remove either the source or the destination values.|
|SRC_COLOR|The value of this stage is multiplied by the source color value.|
|DST_COLOR|The value of this stage is multiplied by frame buffer source color value.|
|SRC_ALPHA|The value of this stage is multiplied by the source alpha value.|
|DST_ALPHA|The value of this stage is multiplied by frame buffer source alpha value.|
|ONE_MINUS_SRC_COLOR|The value of this stage is multiplied by (1 - source color).|
|ONE_MINUS_DST_COLOR|The value of this stage is multiplied by (1 - destination color).|
|ONE_MINUS_SRC_APLHA|The value of this stage is multiplied by (1 - source alpha).|
|ONE_MINUS_DST_APLHA|The value of this stage is multiplied by (1 - destination alpha).|
### Equations
|Equation|Description|
|--|--|
|ADD|Add source and destination together.|
|SUB|Subtract destination from source.|
|REV_SUB|Subtract source from destination.|
|MIN|Use the smaller of source and destination.|
|MAX|Use the larger of source and destination.|
## Culling
`PLUTO_CULL_FACE FRONT`: Don’t render polygons facing away from the viewer (default).

`PLUTO_CULL_FACE BACK`: on’t render polygons facing towards the viewer. Used for turning objects inside-out.

`PLUTO_CULL_FACE OFF`: Disables culling - all faces are drawn. Used for special effects.
## Depth Test
`PLUTO_DEPTH_TEST OFF`: If you’re drawing solid objects, leave this on. If you’re drawing semi transparent effects, switch to `OFF`.

`PLUTO_DEPTH_TEST ALWAYS`: The depth test always passes.

`PLUTO_DEPTH_TEST NEVER`: The depth test never passes.

`PLUTO_DEPTH_TEST LESS`: Passes if the fragment's depth value is less than the stored depth value.

`PLUTO_DEPTH_TEST EQUAL`: Passes if the fragment's depth value is equal to the stored depth value.

`PLUTO_DEPTH_TEST LESS_EQUAL`: Passes if the fragment's depth value is less than or equal to the stored depth value (default).

`PLUTO_DEPTH_TEST GREATER`: Passes if the fragment's depth value is greater than the stored depth value.

`PLUTO_DEPTH_TEST NOT_EQUAL`: Passes if the fragment's depth value is not equal to the stored depth value.

`PLUTO_DEPTH_TEST GREATER_EQUAL`: Passes if the fragment's depth value is greater than or equal to the stored depth value.