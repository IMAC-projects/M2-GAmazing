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

`̀ `bash
cd doxygen
doxygen Doxyfile
`̀ `

## Getting started

`̀ `bash
mkdir build
cd build
cmake ..
make
./bin/GAmazing
`̀ `

