#! /usr/bin/python

import wx
import os
from transf import *

# ---------------------------- Main Frame

class MainFrame(wx.Frame):
    width  = 1044
    height = 788

    def __init__(self, title):
        super(MainFrame, self).__init__(None, -1, title, 
                size=(self.width, self.height), 
                style=wx.CAPTION | wx.CLOSE_BOX)

        hbox = wx.BoxSizer(wx.HORIZONTAL)

        self.Centre()
        self.SetSizer(hbox)

        self.SetUpPanels()

    def SetUpPanels(self):
        self.CPanel = ControlPanel(self, self.width, self.height)
        self.DPanel = DrawPanel(self, self.height)

        self.GetSizer().Add(self.CPanel)
        self.GetSizer().Add(self.DPanel)

    def SetObject(self, o):
        self.DPanel.SetObject(o)

    def CalculateCoord(self, isP, src, plane, r0):
        self.DPanel.CalculateCoord(isP, src, plane, r0)

# ---------------------------- UI User Input

class ControlPanel(wx.Panel):

    def __init__(self, parent, sWidth, sHeight):
        self.width  = sWidth - sHeight
        self.height = sHeight
        self.parent = parent

        super(ControlPanel, self).__init__(parent, 
                size=(self.width, self.height))

        self.SetBackgroundColour('#D9DDDE')
        self.SetUI()

    def SetUI(self):
        font = wx.Font(18, wx.DEFAULT, wx.NORMAL, wx.NORMAL)
        glob = wx.FlexGridSizer(7, 1)

        ##
        # Instructions
        instGroup = wx.StaticBoxSizer(
                wx.StaticBox(self, label='Instructions'), wx.VERTICAL)
        lblInst = wx.StaticText(self, label='Fill the items bellow with the\ncoordinates X,Y and Z\nseparated by one space.',
                size=(self.width-30, -1))

        instGroup.Add(lblInst, 1, wx.ALL, 5)

        ##
        # File picker UI
        fileGroup = wx.StaticBoxSizer(
                wx.StaticBox(self, label='Object File'), wx.VERTICAL)
        self.btFile = wx.Button(self, label='Click here!', size=(self.width-30, -1))
        self.btFile.Bind(wx.EVT_BUTTON, self.SelectObjFile)

        fileGroup.Add(self.btFile, 2, wx.EXPAND | wx.ALL, 5)

        ##
        # Plane 3 points UI
        planeGroup = wx.StaticBoxSizer(
                wx.StaticBox(self, label='Plane'), wx.VERTICAL)

        self.edP = [None, None, None]
        for i in range(3):
            lbl = wx.StaticText(self, label='P%d:' % (i+1))
            self.edP[i] = wx.TextCtrl(self, value='', size=(self.width-30, -1))
            self.edP[i].SetFont(font)

            planeGroup.Add(lbl, 1, wx.ALL, 5)
            planeGroup.Add(self.edP[i], 2, wx.ALL, 5)

        ##
        # Point R0 UI
        opGroup = wx.StaticBoxSizer(
                wx.StaticBox(self, label='Optional'), wx.VERTICAL)
        lblR0 = wx.StaticText(self, label='R0:')
        self.edR0 = wx.TextCtrl(self, value='', size=(self.width-30, -1))
        self.edR0.SetFont(font)

        opGroup.Add(lblR0, 1, wx.ALL, 5)
        opGroup.Add(self.edR0, 2, wx.ALL, 5)
        
        ##
        # Source UI (Camera and Direction Vector)
        srcGroup = wx.StaticBoxSizer(
                wx.StaticBox(self), wx.VERTICAL)
        self.lblSrc = wx.StaticText(self, label='')
        self.edSrc = wx.TextCtrl(self, value='', size=(self.width-30, -1))
        self.edSrc.SetFont(font)

        srcGroup.Add(self.lblSrc, 1, wx.ALL, 5)
        srcGroup.Add(self.edSrc, 2, wx.ALL, 5)

        ##
        # Projection Select
        projGroup = wx.StaticBoxSizer(
                wx.StaticBox(self, label='Projection:'), wx.HORIZONTAL)

        self.btPersp = wx.Button(self, label='Central', size=((self.width-40)/2, -1))
        self.btPara = wx.Button(self, label='Parallel', size=((self.width-40)/2, -1))

        self.btPersp.Bind(wx.EVT_BUTTON, self.CentralToggle)
        self.btPara.Bind(wx.EVT_BUTTON, self.ParallelToggle)

        projGroup.Add(self.btPersp, 1, wx.EXPAND | wx.ALL, 5)
        projGroup.Add(self.btPara, 1, wx.EXPAND | wx.ALL, 5)

        ##
        # Button
        self.btCalc = wx.Button(self, label='Refresh', size=(self.width-20, -1))
        self.btCalc.Bind(wx.EVT_BUTTON, self.OnClick) 

        glob.Add(fileGroup, 1, wx.ALL, 5)
        glob.Add(instGroup, 1, wx.ALL, 5)
        glob.Add(planeGroup, 1, wx.ALL, 5)
        glob.Add(opGroup, 1, wx.ALL, 5)
        glob.Add(srcGroup, 1, wx.ALL, 5)
        glob.Add(projGroup, 1, wx.ALL, 5)
        glob.Add(self.btCalc, 5, wx.EXPAND | wx.ALL, 5)

        self.SetSizer(glob)
        self.CentralToggle(0)

    def CentralToggle(self, ev):
        self.btPersp.Disable()
        self.btPara.Enable(True)
        self.lblSrc.SetLabel('Camera:')

    def ParallelToggle(self, ev):
        self.btPersp.Enable(True)
        self.btPara.Disable()
        self.lblSrc.SetLabel('Direction Vector:')

    def SelectObjFile(self, ev):
        fileSelector = wx.FileDialog(self, 'Open Object File', '.',
                '', 'YK File (*.yk)|*.yk')
        fileSelector.ShowModal()
        path = fileSelector.GetPath()
        fileSelector.Destroy()

        self.GetParent().SetObject(Object(path))
        self.btFile.SetLabel(str(os.path.basename(path)))


    def ValidateInput(self, i):
        if i.strip() == '':
            return False
        else:
            Istr = i.strip().split(' ')
            if len(Istr) != 3:
                return False
            else:
                return True

    def GetListFromInput(self, i):
        L = []
        Istr = i.strip().split(' ')
        for j in range(len(Istr)):
            L.append(int(Istr[j]))
        return L


    def OnClick(self, ev):
        error = False
        P = [[], [], []]
        Src = []
        R0 = None 

        ##
        # Projection type
        isPerspective = not self.btPersp.IsEnabled()

        ##
        # Plane 3 points validation
        for i in range(3):
            error = not self.ValidateInput(self.edP[i].GetValue())
            if not error:
                P[i] = self.GetListFromInput(self.edP[i].GetValue())

        ##
        # R0 validation
        if self.ValidateInput(self.edR0.GetValue()):
            R0 = self.GetListFromInput(self.edR0.GetValue())

        ##
        # Source validation
        error = not self.ValidateInput(self.edSrc.GetValue())
        if not error:
            Src = self.GetListFromInput(self.edSrc.GetValue())

        if error:
            wx.MessageBox('Something went wrong. Fill all the input correctly.',
                    'Error', wx.ICON_ERROR | wx.OK)
        else:
            self.GetParent().CalculateCoord(isPerspective, Src, P, R0)


# ---------------------------- Drawing Screen

class DrawDC(wx.PaintDC):

    def __init__(self, screen):
        super(DrawDC, self).__init__(screen)
        self.SetUpComponents()

    def SetUpComponents(self):
        self.linePen = wx.Pen('#0000ff', 2, wx.PENSTYLE_SOLID)
        self.vertPen = wx.Pen('#ff0000', 5, wx.PENSTYLE_SOLID)

    def DrawSPoint(self, x, y):
        self.DrawLine(x, y, x, y)

    def DrawOLine(self, u, v):
        self.DrawLine(u[0], u[1], v[0], v[1])

    def DrawObject(self, o):
        if o == None or not o.isCalculated:
            return

        oldPen = self.GetPen()

        self.SetPen(self.linePen)
        for face in o.face:
            for (u,v) in face:
                self.DrawOLine(o.devVertex[u], o.devVertex[v])

        self.SetPen(self.vertPen)
        for v in o.devVertex:
            self.DrawSPoint(v[0], v[1])

        self.SetPen(oldPen)

    def DrawGrid(self, width, height):
        step = 20 
        oldPen = self.GetPen()
        self.SetPen(wx.Pen('#ff0000', 1, wx.DOT))

        for i in range(0, width-1, step):
            self.DrawLine(i, 0, i, height-1)
        for i in range(0, height-1, step):
            self.DrawLine(0, i, width-1, i)

        self.SetPen(oldPen)


class DrawPanel(wx.Panel):

    def __init__(self, parent, sHeight):
        self.width  = sHeight
        self.height = sHeight

        super(DrawPanel, self).__init__(parent, 
                size=(self.width, self.height))

        self.o = None

        self.SetUpScreen()
        self.screen.Bind(wx.EVT_PAINT, self.OnPaint)

    def SetUpScreen(self):
        box = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(box)

        self.screen = wx.Panel(self, size=(self.width-20, self.height-20))
        self.screen.SetBackgroundColour('#00ff00') 

        box.Add(self.screen, 1, wx.ALL, 10)

    def OnPaint(self, ev):
        dc = DrawDC(self.screen)

        dc.DrawGrid(self.width, self.height)
        dc.DrawObject(self.o)

    def SetObject(self, o):
        self.o = o

    def CalculateCoord(self, isP, src, plane, r0):
        if self.o != None:
            self.o.CalcForDevice(isP, src, plane, self.width, self.height, r0)
            self.Refresh()
        else:
            wx.MessageBox('Select an object before plotting.', 'Error',
                    wx.ICON_ERROR | wx.OK)

# ---------------------------- Main

if __name__ == "__main__":
    app = wx.App()

    main = MainFrame('Computer Graphics')
    main.Show()

    app.MainLoop()
