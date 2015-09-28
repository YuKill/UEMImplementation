# Projections
This program projects 3d points into a 2d plane by using a transformation matrix. It is implemented in python. In order to execute this program, you should have python 2.7.9. Please, follow the instructions below:

### 1. Executing
##### 1.1 Linux
If you are using linux, you have to do the following commands at the terminal, inside the project root folder:
```
$ chmod +x play.py
$ ./play.py
```

##### 1.2. Windows
If you are using a windows machine, you have to execute the "play.py" file within a python interpreter.

### 2. Using
You have to specify an object file following the description bellow:
```
8 (number of vertices)
0 0 0 (Coordinates of the vertices)
1 0 0
1 1 0
0 1 0
0 0 1
1 0 1
1 1 1
0 1 1
6 (Number of faces)
4 4 4 4 4 4 (Number of vertices for each face)
0 1 2 3 (Vertices that belong to face 0)
0 4 5 1
1 5 6 2
2 6 7 3
0 3 7 4
4 7 6 5
```

The other coordinates (P0, P1, P2, R0 and the Camera  or Direction vector) must be specified separating the X, Y and Z by a ```space```, as mentioned in the  UI.
