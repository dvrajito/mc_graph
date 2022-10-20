#!/usr/bin/env python

# Python interface for the mc_graph project used to configure the c++
# program. Written by D. Vrajitoru, August, 2004.

import tkFileDialog
import Tkinter
import FileDialog
import Pmw
import string
import time
import os.path
import filenames

def get_font():
    return "6x13"

def makeComboBox(parent,test_str,var):
    combo_box=Pmw.ComboBox(parent,labelpos="w",label_text=test_str,
                           selectioncommand = lambda val,obj=var:obj.set(val),
                           entryfield_entry_width = 5)
    combo_box._entryWidget.config(font=get_font())
    combo_box._list.component('listbox').config(font=get_font())
    combo_box.component('label').config(font=get_font())
    combo_box.component('entryfield').setentry(var.get())
    combo_box.pack()
    return combo_box

def makeSpace(parent):
    blank_space=Tkinter.Label(parent,font = "6x9", text=" ")
    blank_space.pack()

class MCGraph(Tkinter.Toplevel):
    def __init__(self):
        aframe=Tkinter.Frame()
        aframe.pack()
        Tkinter.Toplevel.__init__(self,aframe)
        self.title('MCG control')
        self.UserWidgets()
        
        self.grab_set()
        self.focus_set()
        self.wait_window()

    def UserWidgets(self):        
        aframe = Tkinter.Frame(self)
        aframe.pack(fill='x')
        aframe1=Tkinter.Frame(aframe)
        aframe1.pack(side="left",anchor="w")
        
	#file selection dialog
	self.filename = Tkinter.StringVar()
	self.filename.set("Problems/DrawGraph/dg10dv.txt")
        self.file_label = Tkinter.Label(aframe1,text="Graph file: dg10dv.txt")
        self.file_label.pack(anchor="w")
        button = Tkinter.Button(aframe1,text="Set graph file...",
                                command=self.setGraphFile)
        button.pack(anchor="w")

        aframe1=Tkinter.Frame(aframe)
        aframe1.pack(side="right",anchor="e")
        # The widgets for the result file name
        self.result_file=Tkinter.StringVar()
        self.result_file.set("./Results/dg10res.txt")
        self.res_file_label = Tkinter.Label(aframe1,
                                            text="Result file: dg10res.txt")
        self.res_file_label.pack(anchor="e")
        rfile_browser = Tkinter.Button(aframe1,text="Set result file...",
                                       command = self.setResultFile)
	rfile_browser.pack(anchor="e")

        makeSpace(self)

        aframe = Tkinter.Frame(self)
        aframe.pack()
        wrap_frame=Tkinter.Frame(aframe)
        wrap_frame.pack(side="left")
        method_label=Tkinter.Label(wrap_frame, text="Method")
        method_label.pack(side="top",anchor="w")
        
        #choose the method: breadth first or random
        self.method = Tkinter.IntVar()
	self.method.set(0)
        method_rad1 = Tkinter.Radiobutton(wrap_frame,text="Breadth first")
        method_rad2 = Tkinter.Radiobutton(wrap_frame,text="Random")
        method_rad3 = Tkinter.Radiobutton(wrap_frame,text="Tension vector")
	method_rad4 = Tkinter.Radiobutton(wrap_frame,text="Repulsion vector")
        method_rad1["variable"]=self.method 
        method_rad2["variable"]=self.method 
        method_rad3["variable"]=self.method 
        method_rad4["variable"]=self.method 
        method_rad1["value"]=0
        method_rad2["value"]=1
        method_rad3["value"]=2
        method_rad4["value"]=3
        method_rad1.pack(side="top",anchor="w")
        method_rad2.pack(side="top",anchor="w")
        method_rad3.pack(side="top",anchor="w")
        method_rad4.pack(side="top",anchor="w")

        wrap_frame=Tkinter.Frame(aframe)
        wrap_frame.pack(side="left")
        set_label=Tkinter.Label(wrap_frame, text="Settings")
        set_label.pack(side="top",anchor="e")
	self.epsilon = Tkinter.DoubleVar()
	self.epsilon.set(0.1)
        eps_box=makeComboBox(wrap_frame,"Epsilon", self.epsilon)
        eps_box.component('listbox').insert(1,0.05,0.1,0.2,0.5,0.9,1)
        eps_box.pack(side="top",anchor="e")

        self.iterations=Tkinter.IntVar()
        self.iterations.set(10)
        self.iter_box=makeComboBox(wrap_frame,"Number of iterations",
                              self.iterations)
        self.iter_box.pack(side="top",anchor="e")
        self.iter_box.component('listbox').insert(1,100,500,1000,5000,10000)
        
        #self.intern=Tkinter.IntVar()
        #self.intern.set(1)
        #iter_box=makeComboBox(self.UserTop,"Number of interior iterations",
        #                      self.intern)
        #iter_box.component('listbox').insert(1,1,5,10,25,50,100)
        
        self.step=Tkinter.IntVar()
        self.step.set(1)
        step_box=makeComboBox(wrap_frame,"Report step",
                              self.step)
        step_box.component('listbox').insert(1,1,5,10,25,50,100)
        step_box.pack(side="top",anchor="e")

        self.trials=Tkinter.IntVar()
        self.trials.set(1)
        tr_box=makeComboBox(wrap_frame,"Number of trials",
                              self.trials)
        tr_box.component('listbox').insert(1,1,5,10,25,50,100)
        tr_box.pack(side="top",anchor="e")

        makeSpace(self)
        
        # Build the layout from current settings
        self.cvbutton = Tkinter.Button(self,text="Convergence (off) ...",
                                command=self.CallConverge)
        self.cvbutton.pack()
        makeSpace(self)
        
        self.binDir=Tkinter.StringVar()
        self.binDir.set("../bin")
        self.bin=Tkinter.StringVar()
        self.bin.set("../bin/mcg")
        self.findBinDir()
        self.binfile_label = Tkinter.Label(self,text="Bin directory:\n %s"%(self.binDir.get()),font=get_font())
        self.binfile_label.pack()
        self.binfile_browser = Tkinter.Button(self,text="Set bin directory...",
                                           font=get_font(),
                                           command = self.setBinDir)
	self.binfile_browser.pack(fill='x')
        
        # Build the layout from current settings
        button = Tkinter.Button(self,text="Run",
                                command=self.Run)
        button.pack()

        set_button=Tkinter.Button(self,text="Loop...",
                                  command = self.CallLoop)
	set_button.pack()

        set_button=Tkinter.Button(self,text="Quit",
                                  command = self.Quit)
	set_button.pack()

        self.loop_ctrl=None
        self.converge_ctrl = None

        self.converge = Tkinter.IntVar()
        self.converge.set(0)
        self.converge_limit = Tkinter.DoubleVar()
        self.converge_limit.set(0.05)

    def findBinDir(self):
        path=self.binDir.get()
        binfile=os.path.join(path, "mcg")
        if os.path.exists(binfile):
            self.bin.set(binfile)
            return
        else:
            paths = ("../bin/", "./","../","./bin/")
            for path in paths:
                binfile=os.path.join(path, "mcg")
                if os.path.exists(binfile):
                    self.bin.set(binfile)
                    return
            self.binDir.set("")
            self.bin.set("")

    def setBinDir(self):
        fname=FileDialog.FileDialog(self).go()
        if fname:
            self.binDir.set(fname)
            self.bin.set(os.path.join(self.binDir.get(),"mcg"))
            self.binfile_label.config(text="Bin directory:\n %s"%(self.binDir.get()))

    def CallLoop(self):
        self.loop_ctrl=LoopControl(self)

    def CallConverge(self):
        self.converge_ctrl=ConvergeControl(self)

    def Quit(self):
        if self.loop_ctrl:
            self.loop_ctrl.destroy()
        if self.converge_ctrl:
            self.converge_ctrl.destroy()
        self.destroy()

    def update_data(self,event):
	self.data = self.entry_contents.get()

    def setResultFile(self):
        old_name=self.result_file.get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.asksaveasfilename(initialdir="%s" %(old_dir),
                                             initialfile="%s"%(old_file))
        if fname:
            self.result_file.set(fname)
            ffname = os.path.basename(fname)
            self.res_file_label.config(text="Result file: %s"%(ffname))

    def setGraphFile(self):
        old_name=self.filename.get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.askopenfilename(initialdir="%s" %(old_dir),
                                           initialfile="%s"%(old_file))
        if fname:
            self.filename.set(fname)
            ffname = os.path.basename(fname)
            self.file_label.config(text="Graph file: %s"%(ffname))
            
    def WritePrefFile(self, preffile):
        preff = open(preffile, "w")
        preff.write("%s\n" %(self.filename.get()))
        preff.write("%s\n" %(self.result_file.get()))
        preff.write("%g\n" %(self.epsilon.get()))
        preff.write("%d\n" %(self.method.get()))
        preff.write("%d\n" %(self.iterations.get()))
        preff.write("%d\n" %(self.step.get()))
        preff.write("%d\n" %(self.converge.get()))
        preff.write("%g\n" %(self.converge_limit.get()))
        preff.close()
            
    def Run(self):
        if self.binDir.get() != "":
            preffile=os.path.join(self.binDir.get(), "mcg_prefs.txt")
            self.WritePrefFile(preffile)
            command="%s %s" %(self.bin.get(), preffile)
            fout=open(self.result_file.get(), 'a')
            fout.write("Graph file: ")
            fout.write(self.filename.get())
            fout.write("\n\n")
            fout.close()
            time1=time.time()
            for i in range(self.trials.get()):
                os.system(command)
            total_time = time.time() - time1
            fout=open(self.result_file.get(), 'a')
            fout.write("Total time: ")
            fout.write("%f" %(total_time))
            fout.write("\n\n")
            fout.close()
            print "Total time:", total_time
        
class LoopControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Loop Control')
        #self.var=parent.var
        self.top=parent

        self.loop_opt=Tkinter.IntVar()
        self.loop_opt.set(1)
        opt_rad1 = Tkinter.Radiobutton(self,text="epsilon")
        opt_rad2 = Tkinter.Radiobutton(self,text="all graph files")
        opt_rad3 = Tkinter.Radiobutton(self,text="start from reference file")
        opt_rad1["variable"]=self.loop_opt
        opt_rad2["variable"]=self.loop_opt
        opt_rad3["variable"]=self.loop_opt
        opt_rad1["value"]=0
        opt_rad2["value"]=1
        opt_rad3["value"]=2
        opt_rad1.pack()
        opt_rad2.pack()
        opt_rad3.pack()

        self.ep_start = Tkinter.DoubleVar()
        self.ep_start.set(self.top.epsilon.get())
        self.ep_end = Tkinter.DoubleVar()
        self.ep_end.set(self.top.epsilon.get())
        self.ep_step = Tkinter.DoubleVar()
        self.ep_step.set(self.top.epsilon.get())

        svbox=makeComboBox(self,"Starting value",self.ep_start)
        svbox.component('listbox').insert(1,0,0.1,0.01,1,2,3)
        evbox=makeComboBox(self,"Ending value",self.ep_end)
        evbox.component('listbox').insert(1,0.2,0.5,1)
        stbox=makeComboBox(self,"Step",self.ep_step)
        stbox.component('listbox').insert(1,0.1,0.01)

        quit_button=Tkinter.Button(self,text="Run", command = self.Run)
	quit_button.pack()

        quit_button=Tkinter.Button(self,text="Cancel", command = self.Quit)
	quit_button.pack()

    def Run(self):
        if self.loop_opt.get()==0:
            if (self.ep_end.get() < self.ep_start.get() and
                self.ep_step.get() >= 0):
                self.ep_end.set(self.ep_start.get())
            if self.ep_step.get() == 0:
                self.ep_step.get(self.ep_start.get())
            epsilon=self.ep_start.get()
            old_epsilon = self.top.epsilon.get()
            old_res_file = self.top.result_file.get()
            while epsilon <= self.ep_end.get():
                self.top.epsilon.set(epsilon)
                res_file = filenames.append_name(old_res_file, epsilon)
                self.top.result_file.set(res_file)
                self.top.Run()
                epsilon = epsilon + self.ep_step.get()
            self.top.epsilon.set(old_epsilon)
            self.top.result_file.set(old_res_file)
        else:
            initial_file = self.top.filename.get()
            the_path = os.path.dirname(initial_file)
            the_file = os.path.basename(initial_file)
            init_ext = filenames.get_extension(the_file)
            files = os.listdir(the_path)
            files.sort()
            i=0
            if self.loop_opt.get()==2:
                while (i < len(files)) and files[i] != the_file:
                    i=i+1
            j=i
            res_file = self.top.result_file.get()
            path_res=os.path.dirname(res_file)
            res_base=os.path.basename(res_file)
            epsilon = self.top.epsilon.get()
            if self.loop_opt.get()==1:
                start = 1
            else:
                start = 0
            while (j < len(files)):
                extension = filenames.get_extension(files[j])
                if not start:
                    if files[j] == the_file:
                        start = 1
                if extension == init_ext and start:
                    res_new=os.path.join(path_res, files[j])
                    rfilename = filenames.append_name(res_new, "r")
                    problem = os.path.join(the_path, files[j])
                    self.top.filename.set(problem)
                    self.top.result_file.set(rfilename)
                    self.top.Run()
                j=j+1
            self.top.filename.set(initial_file)
            self.top.result_file.set(res_file)

    def Quit(self):
        self.top.loop_ctrl=None
        self.destroy()

class ConvergeControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Convergence Control')
        self.top=parent
        
        converge_rad1 = Tkinter.Radiobutton(self,text="Convergence Check Off")
        converge_rad2 = Tkinter.Radiobutton(self,text="Convergence Check On")
        converge_rad1["variable"]=self.top.converge
        converge_rad2["variable"]=self.top.converge
        converge_rad1["value"]=0
        converge_rad2["value"]=1
        converge_rad1.pack()
        converge_rad2.pack()
        
        lim_box=makeComboBox(self,"Convergence limit",
                             self.top.converge_limit)
        lim_box.component('listbox').insert(1,0.01,0.02,0.05,0.09,0.1)
        lim_box.pack()

        quit_button=Tkinter.Button(self,text="Ok", command = self.Quit)
	quit_button.pack()

    def Quit(self):
        self.top.loop_ctrl=None
        if self.top.converge.get():
            self.top.cvbutton.config(text="Convergence (on) ...")
            self.top.iter_box.component('label').config(text="      Max iterations")
        else:
            self.top.cvbutton.config(text="Convergence (off) ...")
            self.top.iter_box.component('label').config(text="Number of iterations")
        self.destroy()

if __name__ == '__main__' :
    mcg = MCGraph()
