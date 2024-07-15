# graphics
some raytracing as a small project for fun lol  
renders an image using GL/glut with each R, G and B on a separate ray to (in theory) allow colourful refractions  
have a look at the code, but i wouldnt really want to use it anywhere (not bug free, not feature complete to my liking, slow as hecc but eh)  

# features!
light sources, reflections, refractions, diffusions (nothing fancy, just correct randomness; you need to spawn a lot of rays for it to look correct statistically) (and combinations of these on the same object) on balls and triangles  
optimizations? basically none i suppose xd (it slow and takes a bunch of cputime)  
basic but still sorta nice ray scheduler for each thread

# example render of a.txt
![image](https://github.com/user-attachments/assets/bf88bbcc-45d3-42c0-93c2-2c7e0d9f54cf)
in this image: bright light source white ball, lesser light source red ball, reflecting and diffusing green ball, reflecting purble ball, completely reflecting ball (behind cam), refracting ball  
the diffused area gets more beautiful with 10k rays, but that is a render over night unfortunately :|

# usage
`git clone` whatever  
compile using `g++ object.cpp ray.cpp objects.cpp load.cpp render.cpp main.cpp -lglut -lGLU -lGL` (written here for my own sanity)  
run `./a.out`, then select a.txt f.e with `a` and enter  
this will use every cpu thread your machine has to the fullest for probably a while (depending on the complexity (mainly resolution (duh) and if there are diffused parts)) (i dont have a gpu ;-;)

# config
i (int), d (double), h (hex (but int will probably work))  
you have to fill in every line with value `0` or something (and trailing whitespace) for a total of exactly 16 ` ` because the system is dumb  
you cannot change the cam position via txt at this time, edit it in main.cpp :}  
the first int decides which type of data it is:  
-1: window and renderer info  
0: ball  
1: triangle  

### -1
i width, i height, i max amt of rays, h background colour, ? ?? (idk, unused yet?), ? do diffusion? (unused), i anti-aliasing type (0 x1, 1 x2, 2 x2, 3 x4, 4 x4, 5 x16), amount of diffusion rays (to render on a pixel when a diffused area is encountered)

### 0
h colour, d light intensity, f diffusion %, f reflection %, f refraction %, f refraction index, f x, f y, f z, f radius

### 1
h colour, d light intensity, f diffusion %, f reflection %, f refraction %, f refraction index, f x1, f y1, f z1, f x2, f y2, f z2, f x3, f y3, f z3

diffusion + reflection + refraction <= 1.0, the rest is colour  
a light intensity of 0 will turn the object black, default is 1  
(also just use/read the included ones :))
