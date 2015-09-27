#! /usr/bin/python

import math

class Object:

    def __init__(self, fName=None):

        self.nVertex = 0
        self.nFaces = 0

        self.devVertex = []
        self.vertex = []
        self.face = []
        self.isCalculated = False

        if fName != None:
            self.ReadInput(fName)

    def ReadInput(self, fName):
        with open(fName) as f:
            nVertex = int(f.readline().rstrip('\n'))

            for i in range(nVertex):
                pos = f.readline().rstrip('\n').split()
                self.vertex.append([int(pos[0]), int(pos[1]), int(pos[2]), 1])

            nFaces = int(f.readline().rstrip('\n'))
            
            f.readline()
            for i in range(nFaces):
                vertices = f.readline().rstrip('\n').split()
                firstVert = -1
                lastVert = -1
                self.face.append([])
                for v in vertices:
                    if firstVert == -1:
                        firstVert = int(v)
                    if lastVert != -1:
                        self.face[i].append((lastVert, int(v)))
                    lastVert = int(v)
                self.face[i].append((firstVert, lastVert))

    def CalcNormalVec(self, plane):
        u = [0, 0, 0]
        v = [0, 0, 0]
        for i in range(len(plane[0])):
           u[i] = plane[0][i] - plane[1][i] 
           v[i] = plane[2][i] - plane[1][i]

        n = [0, 0, 0]
        n[0] = (u[1] * v[2]) - (u[2] * v[1])
        n[1] = (u[2] * v[0]) - (u[0] * v[2])
        n[2] = (u[0] * v[1]) - (u[1] * v[0])

        return n

    def GetPerspectiveMatrix(self, c, n, d):
        return  [[d[0]+(c[0]*n[0]),        c[0]*n[1],        c[0]*n[2], -c[0]*(d[0]+d[1])],
                 [       c[1]*n[0], d[0]+(c[1]*n[1]),        c[1]*n[2], -c[1]*(d[0]+d[1])],
                 [       c[2]*n[0],        c[2]*n[1], d[0]+(c[2]*n[2]), -c[2]*(d[0]+d[1])], 
                 [            n[0],             n[1],             n[2],             -d[1]]] 

    def GetParallelMatrix(self, src, n, d):
        return  [[d[1]-(src[0]*n[0]),       -src[0]*n[1],       -src[0]*n[2], src[0]*d[0]],
                 [      -src[1]*n[0], d[1]-(src[1]*n[1]),       -src[1]*n[2], src[1]*d[0]],
                 [      -src[2]*n[0],       -src[2]*n[1], d[1]-(src[2]*n[2]), src[2]*d[0]], 
                 [                 0,                  0,                  0,        d[1]]] 

    def Multiply(self, M, v):
        newV = []
        for line in M:
            elem = 0.0
            for i in range(len(v)):
                elem += (line[i] * v[i])
            newV.append(elem)

        return newV


    def CalcForDevice(self, isPerspective, src, plane, dWidth, dHeight, r0=None):
        if r0 == None:
            r0 = plane[0]

        n = self.CalcNormalVec(plane)
        self.devVertex = []
        
        ## 
        # Defining transformation Matrix
        d = [0, 0]
        for i in range(3):
            if isPerspective:
                r0[i] -= src[i]
            d[0] = d[0] + (r0[i] * n[i])
            d[1] = d[1] + (src[i] * n[i])

        if isPerspective:
            M = self.GetPerspectiveMatrix(src, n, d)
        else:
            M = self.GetParallelMatrix(src, n, d)
        
        ## 
        # Calculating projection window coordinates
        for v in self.vertex:
            newVertex = self.Multiply(M, v) 

            for i in range(3):
                newVertex[i] /= float(newVertex[3])

            self.devVertex.append(newVertex[:3])

        ##
        # Transforming to plane coordinates 
        r = math.sqrt((n[0]*n[0])+(n[1]*n[1]))
        print self.devVertex
        if r != 0:
            T = [[n[1]/r, -n[0]/r, 0],
                 [n[0]/r,  n[1]/r, 0],
                 [     0,       0, 1]]

            for i in range(len(self.devVertex)):
                self.devVertex[i] = self.Multiply(T, self.devVertex[i])

            n = self.Multiply(T, n)

        r = math.sqrt((n[2]*n[2])+(n[1]*n[1]))
        if r != 0:
            T = [[1,      0,       0],
                 [0, n[2]/r, -n[1]/r],
                 [0, n[1]/r,  n[2]/r]]

            for i in range(len(self.devVertex)):
                self.devVertex[i] = self.Multiply(T, self.devVertex[i])
                self.devVertex[i][2] = 1

        ##
        # Geting max and min Xs and Ys
        maxInt = 9223372036854775807
        x = [maxInt, -maxInt]
        y = [maxInt, -maxInt]
        for v in self.devVertex:
            if x[0] > v[0]:
                x[0] = v[0]
            if x[1] < v[0]:
                x[1] = v[0]

            if y[0] > v[1]:
                y[0] = v[1]
            if y[1] < v[1]:
                y[1] = v[1]

        x[0] -= 0.2 * abs(x[1]-x[0])
        y[0] -= 0.2 * abs(y[1]-y[0])
        x[1] += 0.2 * abs(x[1]-x[0])
        y[1] += 0.2 * abs(y[1]-y[0])

        ## 
        # Calculating device coordinates
        s = [dWidth/(x[1]-x[0]), dHeight/(y[1]-y[0])]
        
        T = [[s[0],     0, -(x[0]*s[0])],
             [   0, -s[1],  (y[0]*s[1])],
             [   0,     0,            1]]

        for i in range(len(self.devVertex)):
            self.devVertex[i] = self.Multiply(T, self.devVertex[i])
            
        ## 
        # Moving object back
        y[0] = maxInt
        y[1] = -maxInt
        for v in self.devVertex:
            if y[0] > v[1]:
                y[0] = v[1]
            if y[1] < v[1]:
                y[1] = v[1]

        for i in range(len(self.devVertex)):
            self.devVertex[i][0] = round(self.devVertex[i][0])
            self.devVertex[i][1] = round(self.devVertex[i][1] + abs(y[0] + y[1]))
            self.devVertex[i][2] = round(self.devVertex[i][2])

        self.isCalculated = True

    def Print(self):
        print "Vertex: ", len(self.vertex)
        print "Faces: ", len(self.face)
        print self.face
        print self.vertex
        print self.devVertex

if __name__ == '__main__':
    cube = Object('cube')

    cube.CalcForDevice([0, 10, 20000], [[10, 2, 0],[5, 3, 0],[7, 9, 0]], 768, 768, [10, 5, 0])
    cube.Print()
    
