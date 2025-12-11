A computer draws something by drawing a shitton of tiny triangles and using coloring some differently.
    A bit of an oversimplification
    
A triangle can be defined by 3 vertecies, one for each corner. In a 2D coordinate system, each corner/vertecie can be represented by an x and y, which would give 6 values that should be represented as an image. Therfore, all you need to do is find out what pixels are mostly contained by the triangle, then fill them with colour. This is what the graphics pipeline does.

The graphics pipeline is a linear sequence of stages  where each stage takes input (data), performs some operation(s), and outputs the transformed data as input to the next stage, like a factory assembly line in that each stage raw data is refined into something closer to resembling the final image.

The first stage is the input assembler, which takes as input a list of numbers and groups them together into geometry. For the triangle, the first six numbers are used to form the first triangle, and the 7-12th to form the second. 

Second, the vertex shader
    tHE vertex shader processes each vertex individually and performs transformations such as rotations and translations.

Third, rasterisation:
    The rasterisation stage breaks up the geometry for each pixel the triangle overlaps.

Fourth, fragment shader:
    The fragment shader processes each fragment individually and outputs qualities such as colour by using interpolated data from things  like textures, normals, and lighting.

Fifth, colour blending:
    The colour blending stage applies operations to mix the values from multiple fragments that correspond to the same pixel in the final image.
        
There are 2 types of stages; fixed function and programmable. The input assembler, rasterisation, and colour blending stages are all fixed function stages, which means you have less control over what operations these stages perform, but you can configure them by setting variabls that modify their behaviour. Programmable stages meean you can upload your own code to be executed by the GPU. The programs that live in the GPU are called shaders, and are written in a C-like language called glsl. 
