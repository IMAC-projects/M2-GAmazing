# GAmazing

GAmazing is a playground in order to show that geometric algebra is amazing.

The aim of this project is to realize multiple forms and fractals with geometric algebra using [GARAMON](https://github.com/vincentnozick/garamon) to obtain beautiful things.

<p align="center">
  <img src="https://media.giphy.com/media/XRnbDusSE2cBG/source.gif" alt="adventure time gif, jake and finn amazed"/></br>
  <i>Amazing geometric algebra</i>
</p>

## How to install Garamon ?

Download this [GARAMON](https://github.com/vincentnozick/garamon)

```bash
cd garamon
mkdir build
cd build
cmake ..
sudo make install
```

## What is the dependencies of the project ?

- CMake (>3.10)
- Eigen (>3.3.4)
- C3GA
- OpenGL
- glm
- glew
- glfw
- (optional) Doxygen

## Generate documentation (optional)

```bash
cd doxygen
doxygen Doxyfile
```

## Getting started

```bash
mkdir build
cd build
cmake ..
make
./bin/GAmazing
```

## Explanation

### Square

4 CGA points a, b, c, d

```c3ga::Mvec<float> a = c3ga::point<float>(-0.5f, -0.5f, 0);
c3ga::Mvec<float> b = c3ga::point<float>(0.5f, -0.5f, 0);
c3ga::Mvec<float> c = c3ga::point<float>(0.5f, 0.5f, 0);
c3ga::Mvec<float> d = c3ga::point<float>(-0.5f, 0.5f, 0);
```

#### Constrution of the plane 

```
c3ga::Mvec<float> plane = a ^ b ^ c ^ c3ga::ei<float>();
```

#### Normals construction

```
c3ga::Mvec<float> normal = plane * a;
```

#### Retriving vertices position in euclidean space

```c3ga::Mvec<float> a0 = a / a[c3ga::E0];
c3ga::Mvec<float> b0 = b / b[c3ga::E0];
c3ga::Mvec<float> c0 = c / c[c3ga::E0];
c3ga::Mvec<float> d0 = d / d[c3ga::E0];
glm::vec3 aPos = { a0[c3ga::E1], a0[c3ga::E2], a0[c3ga::E3] };
glm::vec3 bPos = { b0[c3ga::E1], b0[c3ga::E2], b0[c3ga::E3] };
glm::vec3 cPos = { c0[c3ga::E1], c0[c3ga::E2], c0[c3ga::E3] };
glm::vec3 dPos = { d0[c3ga::E1], d0[c3ga::E2], d0[c3ga::E3] };
```
          

### Bouncing circle

#### Simple circle

```c3ga::Mvec<double> C = c3ga::point<double>(1,2,2.0) 
                      ^ c3ga::point<double>(2,0,2.0) 
                      ^ c3ga::point<double>(1,0,2.0);  
```

Circle is displayed with a point x, y and a radius.

extractDualCircle use the center as x and y coord. The radius is extracted for the dual circle too and multiply by 100, otherwise the value was too small.

### Flower

#### Simple sphere

```
c3ga::Mvec<double> sphere = c3ga::point<double>(0,0,2.0) 
                          ^ c3ga::point<double>(2,0,2.0) 
                          ^ c3ga::point<double>(1,1,2.0) 
                          ^ c3ga::point<double>(1,0,3.0);
```

We dualize the sphere and extract some parameters.

```
sphere /= sphere[c3ga::E0]; // back to correct scale
double squareRadius = sphere | sphere;
double radius = sqrt(squareRadius);
c3ga::Mvec<double> center = sphere;
center /= center[c3ga::E0];
c3ga::Mvec<double> direction;
direction[c3ga::E0] =  center[c3ga::E0];
```

The direction is used to compute the angle of the flower and the radius is obviously for the flower's radius.

### Fractal 1

I played with some multivectors.

```
c3ga::Mvec<double> p;
p[c3ga::E0] = c3ga::e2<double>();
p[c3ga::E1] = imageXf;
p[c3ga::E2] = imageYf;

c3ga::Mvec<double> x = p;
for (int i = 0; i < maxIter; i++) {
    x = x * x + c; // n = 2
    if (x.norm()/10.0f > 20) {
        break;
    }
}
```

Each value is then stored into a buffer

```
float valF = (x.norm() / 1e10f) * 100;
float val = (valF > 255) ? 255 : (valF);
rgbBuffer[idx + 0] = 0.0f;
rgbBuffer[idx + 1] = int(val/5);
rgbBuffer[idx + 2] = val;
```

### Fractal 2

Same as above except that we play with the condition on x and the buffer storage.
