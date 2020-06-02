# Relatively Robust Divide and Conquer 2D Delaunay Construction Algorithm

Copyright 2005-2017 (c) Wael El Oraiby, All rights reserved.

### Introduction

This is a library that builds a 2D Delaunay Construction using a divide and conquer algorithm very similar to the one by Guibas and Stolfi.

Given a set of inputs points, the program will output Delaunay faces (not necessarily triangles, as long as the points are circumscribed to a circle).

### Contributors
* Raja Lehtihet (bug fixes and suggestions)
* Radovan Bast (CMake build system)

Thanks to:
* Dominique Schmitt and Jean-Claude Spehner for their valuable help and support.
* Jonathan Richard Shewchuk for the robust computational geometry predicates.


### Configuration and compilation using CMake and Make

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```


### Usage

The algorithm builds the 2D Delaunay triangulation given a set of points of at least
3 points using:

    delaunay2d_t* delaunay2d_from(del_point2d_t *points, unsigned int num_points);

- `points`	: point set given as a sequence of tuple x0, y0, x1, y1, ....
- `num_points`	: number of given point
- `pred`		: the incircle predicate.
- `return value`	: the created topology

the returned `delaunay2d_t` structures contains:
- `num_points`	: the input point count
- `points`	: a copy of the input points
- `num_faces`	: the output face count
- `faces`	: the output faces indices (faces are not necessarily triangles). The first face is the external face.

You have to release the structure by calling `delaunay2d_release`.

See the provided example if you want more information. The example requires Qt 5 however.

### Triangulated Output
A new feature is the ability to triangulate the output of the `delaunay2d` function. The function for doing so is:

    tri_delaunay2d_t* tri_delaunay2d_from(delaunay2d_t *orig);
    
This will create a new structure that has the following fields:

- `num_points`   : input points count
- `points`  : a copy of the input points (from orig)
- `num_triangles`   : number of triangles
- `tris`    : the triangles indices v0,v1,v2, v0,v1,v2 ....

Release the `tri_delaunay2d_t` structure by calling `tri_delaunay2d_release`.
 
### Robustness
Currently robustness is achieved by using 64 bits precision inputs and computation using 80 bits. It's possible to achieve the maximum fast robustness using __float128 for computation (without using a slow BigFloat library). This however is not supported with ARM.

Historical Note: Previous version of delaunay used the Predicates from Jonathan Richard Shewchuk. The code is however unstable when compiled with gcc with -m32 and run on x64 machines. As such the code was removed.

### Examples
![random](https://github.com/eloraiby/delaunay/raw/master/images/random.png)
![grid](https://github.com/eloraiby/delaunay/raw/master/images/grid.png)
![vertical and horizontal lines](https://github.com/eloraiby/delaunay/raw/master/images/vertical_horizontal.png)
![circles](https://github.com/eloraiby/delaunay/raw/master/images/circles.png)

### Notes

The implementation is relatively robust (take a look at the pictures above, some of these cases will crash most freely available implementations), in case of input or predicate floating point rounding errors, it will assert. Note that while the robustness is more than enough for most application, there is still room for improvement.

### License

The delaunay.c source code is licensed under Affero GPL version 3.0 or later. While I think this license is the best for OSS, you can obtain a more permissive license suitable for closed source programs for a modest fee. You can contact me for that [contact me](https://github.com/eloraiby/eloraiby.github.com/raw/master/email.svg).



