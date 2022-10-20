#!/usr/bin/env python
import Tkinter
import tkFileDialog
import FileDialog
import Pmw
import os
import string
import types
import os.path
import whrandom
from process_res import *

def get_font():
    return "6x13"

class GAcontrol(Tkinter.Toplevel):
    def __init__(self,parent=None):
        self.generator = whrandom.whrandom()
        if parent:
            aframe = parent
        else:
            aframe=Tkinter.Frame()
            aframe.pack()
        Tkinter.Toplevel.__init__(self,aframe)
        self.title('GA control')
        self.filename=Tkinter.StringVar()
        self.filename.set("../bin/GAPrefs.txt")
        self.binDir=Tkinter.StringVar()
        self.binDir.set("../bin")
        self.bin=Tkinter.StringVar()
        self.bin.set("../bin/GAD")
        self.file_label = Tkinter.Label(self,text="Bin directory:\n %s"%(self.binDir.get()),font=get_font()) 
        self.file_label.pack()
        self.file_browser = Tkinter.Button(self,text="Set bin directory...",
                                           font=get_font(),
                                           command = self.setBinDir)
	self.file_browser.pack(fill='x')

        oper_button=Tkinter.Button(self,text="Operator options",
                                   font=get_font(),
                                   command = self.CallOpers)
	oper_button.pack(fill='x')

        eval_button=Tkinter.Button(self,text="Evaluation options",
                                   font=get_font(),
                                   command = self.CallEvals)
	eval_button.pack(fill='x')

        oper_button=Tkinter.Button(self,text="Running options",
                                   font=get_font(),
                                   command = self.CallRun)
	oper_button.pack(fill='x')

        set_button=Tkinter.Button(self,text="Auto set",
                                  font=get_font(),
                                  command = self.AutoSet)
	set_button.pack()

        run_frame=Tkinter.Frame(self)
        run_frame.pack()
        run_left=Tkinter.Frame(run_frame)
        run_left.pack(side='left')
        run_right=Tkinter.Frame(run_frame)
        run_right.pack(side='right')

        set_button=Tkinter.Button(run_right,text="Loop",
                                  font=get_font(),
                                  command = self.CallLoop)
	set_button.pack()

#        quit_button=Tkinter.Button(run_left,text="Save",
#                                   font=get_font(),command = self.Save)
#	quit_button.pack()

#        quit_button=Tkinter.Button(run_right,text="Run",
#                                   font=get_font(),command = self.Run)
#	quit_button.pack()

#        quit_button=Tkinter.Button(run_left,text="Quit",
#                                   font=get_font(),command = self.Quit)
#	quit_button.pack()
        
        self.initVars()
        self.Read()
        self.oper_ctrl=None
        self.eval_ctrl=None
        self.run_ctrl=None
        self.loop_ctrl=None
        
        self.grab_set()
        self.focus_set()
        self.wait_window()

    def setBinDir(self):
        fname=FileDialog.FileDialog(self).go()
        if fname:
            self.binDir.set(fname)
            self.filename.set(os.path.join(self.binDir.get(),"GAPrefs.txt"))
            self.bin.set(os.path.join(self.binDir.get(),"GAD"))
            self.file_label.config(text="Bin directory:\n %s"%(self.binDir.get()))
            self.Read()

    def Read(self):
        try:
            input=open(self.filename.get(),"r")
            line=input.readline()
            while line:
                x=string.split(line,'\n')[0]
                if self.var.has_key(x):
                    line=input.readline()
                    y=string.split(line,'\n')[0]
                    if type(self.var[x].get()) == types.IntType:
                        self.var[x].set(string.atoi(y))
                    elif type(self.var[x].get()) == types.FloatType:
                        self.var[x].set(string.atof(y))
                    else:
                        self.var[x].set(y)
                else:
                    print "unknown key: ",line
                    line=input.readline()
                line=input.readline()
            input.close()
        except:
            print "Could not read the preference file: set the bin directory"
        #self.PrintVars()

    def Save(self):
        if self.randseed.get() == 0:
            seed=int(self.generator.random()*1000)
            self.var["random seed"].set(seed)
            self.ReConfigure()
        output=open(self.filename.get(),"w")
        for x in self.var.keys():
            save_data=self.var[x].get()
            output.write(x)
            output.write("\n")
            if type(save_data) == types.IntType:
                output.write("%d" %(save_data))
            elif type(save_data) == types.FloatType:
                output.write("%f" %(save_data))
            else:
                output.write(save_data)
            output.write("\n")
        output.close()

    def Run(self):
        cmd = "cd "+self.binDir.get()+";GAD&"
        print "running ", cmd
        self.Save()
        os.system(cmd)
        #os.system("cd ../python_script")

    def Run_seq(self):
        cmd = "cd "+self.binDir.get()+";GAD"
        print "running ", cmd
        self.Save()
        os.system(cmd)
        #os.system("cd ../python_script")

    def Quit(self):
        #self.Save()
        self.destroy()

    def PrintVars(self):
        print "options:"
        for x in self.var.keys():
            print x,": ",self.var[x].get()

    def initVars(self):
        self.var={}
        self.var["inferior limit"]=Tkinter.DoubleVar()
        self.var["inferior limit"].set(0.0)
        self.var["superior limit"]=Tkinter.DoubleVar()
        self.var["superior limit"].set(1.0)
        self.var["probability of mutation"]=Tkinter.DoubleVar()
        self.var["probability of mutation"].set(0.0)
        self.var["probability of crossover"]=Tkinter.DoubleVar()
        self.var["probability of crossover"].set(1.0)
        self.var["individual size"]=Tkinter.IntVar()
        self.var["individual size"].set(10)
        self.var["population size"]=Tkinter.IntVar()
        self.var["population size"].set(10)
        self.var["generation number"]=Tkinter.IntVar()
        self.var["generation number"].set(10)
        self.var["history"]=Tkinter.IntVar()
        self.var["history"].set(0)
        self.var["history file"]=Tkinter.StringVar()
        self.var["history file"].set("")
        self.var["reproduction form"]=Tkinter.IntVar()
        self.var["reproduction form"].set(2)
        self.var["crossover form"]=Tkinter.StringVar()
        self.var["crossover form"].set("combined")
        self.var["diss form"]=Tkinter.StringVar()
        self.var["diss form"].set("and")
        self.var["combined form"]=Tkinter.StringVar()
        self.var["combined form"].set("balanced")
        self.var["crossover sites"]=Tkinter.IntVar()
        self.var["crossover sites"].set(2)
        self.var["swap probability"]=Tkinter.DoubleVar()
        self.var["swap probability"].set(0.5)
        self.var["evaluation form"]=Tkinter.IntVar()
        self.var["evaluation form"].set(1)
        self.var["function number"]=Tkinter.IntVar()
        self.var["function number"].set(0)
        self.var["transformation number"]=Tkinter.IntVar()
        self.var["transformation number"].set(0)
        self.var["problem file"]=Tkinter.StringVar()
        self.var["problem file"].set("")
        self.var["var number"]=Tkinter.IntVar()
        self.var["var number"].set(1)
        self.randseed = Tkinter.IntVar()
        self.randseed.set(0)
        self.var["random seed"]=Tkinter.IntVar()
        self.var["random seed"].set(10)
        self.var["number of trials"]=Tkinter.IntVar()
        self.var["number of trials"].set(1)
        self.var["number of reports"]=Tkinter.IntVar()
        self.var["number of reports"].set(1)
        self.var["result file"]=Tkinter.StringVar()
        self.var["result file"].set("res.txt")
        self.var["encode"]=Tkinter.IntVar()
        self.var["encode"].set(0)
        self.setHCsize()

    def setHCsize(self):
        hc_prob=[9,10,11,12,13,14,15,20,25,30,
                 50,60,70,80,90,100,110,120,130,140,150]
        hc_size=[36,45,55,66,78,91,105,122,153,173,
                 246,353,462,578,782,990,1302,1602,1820,2125,3136]
        self.hc_size={}
        for i in range(len(hc_prob)):
            self.hc_size[hc_prob[i]]=hc_size[i]

    def setIndSize(self):
        if self.var["evaluation form"].get()==1:
            self.var["individual size"].set(self.var["var number"].get()*
                                            10)
        elif self.var["function number"].get()==15:
            problem=string.split(self.var["problem file"].get(),'/')
            problem=problem[len(problem)-1]
            problem=string.split(problem,'c')[1]
            problem=string.atoi(string.split(problem,'d')[0])
            if self.hc_size.has_key(problem):
                self.var["individual size"].set(self.hc_size[problem])
        elif self.var["function number"].get()<11:
            self.var["individual size"].set(300)

    def setVarNr(self):
        if self.var["evaluation form"].get()==0:
            self.var["var number"].set(1)
        else:
            fnr=self.var["function number"].get()
            if fnr in (2,5,9,10):
                self.var["var number"].set(2)
            elif fnr ==1:
                self.var["var number"].set(3)
            elif fnr in (3,6,7,8):
                self.var["var number"].set(5)
            elif fnr ==4:
                self.var["var number"].set(30)

    def setLimits(self):
        if self.var["evaluation form"].get()==0:
            self.var["inferior limit"].set(0.0)
            self.var["superior limit"].set(100)
        else:
            fnr=self.var["function number"].get()
            if fnr in (1,3,6):
                self.var["inferior limit"].set(-3)
                self.var["superior limit"].set(5.11)
            elif fnr in (7,8):
                self.var["inferior limit"].set(-300)
                self.var["superior limit"].set(511)
            elif fnr ==2:
                self.var["inferior limit"].set(-1)
                self.var["superior limit"].set(2.047)
            elif fnr in (9,10):
                self.var["inferior limit"].set(-50)
                self.var["superior limit"].set(100)
            elif fnr ==4:
                self.var["inferior limit"].set(-0.5)
                self.var["superior limit"].set(1.27)
            elif fnr ==5:
                self.var["inferior limit"].set(-40)
                self.var["superior limit"].set(65.535)

    def ReConfigure(self):
        if self.oper_ctrl:
            self.oper_ctrl.ReConfigure()
        if self.eval_ctrl:
            self.eval_ctrl.ReConfigure()
        if self.run_ctrl:
            self.run_ctrl.ReConfigure()

    def AutoSet(self):
        self.setVarNr()
        self.setIndSize()
        self.setLimits()
        self.ReConfigure()

    def CallOpers(self):
        self.oper_ctrl=OperatorControl(self)
        
    def CallEvals(self):
        self.eval_ctrl=EvaluationControl(self)
        
    def CallRun(self):
        self.run_ctrl=RunControl(self)

    def CallLoop(self):
        self.loop_ctrl=LoopControl(self)
        
class two_setit:
    def __init__(self, var1, var2, value1, value2):
        self.__value1 = value1
        self.__value2 = value2
        self.__var1 = var1
        self.__var2 = var2
    def __call__(self, *args):
        self.__var1.set(self.__value1)
        self.__var2.set(self.__value2)
        
class one_setit:
    def __init__(self, var, value):
        self.__value = value
        self.__var = var
    def __call__(self, *args):
        self.__var.set(self.__value)

def makeOptionMenu(parent,text_str,option_str,var):
    menu_fr=Tkinter.Frame(parent)
    menu_fr.pack(side='top')
    menu_label=Tkinter.Label(menu_fr,font=get_font(),text=text_str)
    menu_label.pack(side='left')
    menu_var=Tkinter.StringVar()
    menu_var.set("")
    cr_menu=Tkinter.OptionMenu(menu_fr,menu_var,0)
    cr_menu["menu"].config(font = get_font())
    cr_menu.config(font = get_font())
    cr_menu["menu"].delete(0)
    for i in range(len(option_str)):
        cr_menu["menu"].add_command(label=option_str[i],
                                    command=two_setit(var,menu_var,i,
                                                      option_str[i]))
    cr_menu.pack(side='right')
    cr_menu["menu"].invoke(var.get())
    return cr_menu

def makeStringMenu(parent,text_str,option_str,var):
    menu_fr=Tkinter.Frame(parent)
    menu_fr.pack(side='top')
    menu_label=Tkinter.Label(menu_fr,font=get_font(),text=text_str)
    menu_label.pack(side='left')
    cr_menu=Tkinter.OptionMenu(menu_fr,var,0)
    cr_menu["menu"].config(font = get_font())
    cr_menu.config(font = get_font())
    cr_menu["menu"].delete(0)
    for opt in option_str:
        cr_menu["menu"].add_command(label=opt,
                                    command=one_setit(var,opt))
    cr_menu.pack(side='right')
    cr_menu["menu"].invoke(var.get())
    return cr_menu

def makeComboBox(parent,text_str,var):
    combo_box=Pmw.ComboBox(parent,labelpos="w",label_text=text_str,
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
    
class OperatorControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Operator options')
        self.var=parent.var
        self.top=parent
        pc=makeComboBox(self,"Probability of crossover",
                        self.var["probability of crossover"])
        pc.component('listbox').insert(1,0.0,0.8,1.0)
        pm=makeComboBox(self,"Probability of mutation",
                        self.var["probability of mutation"])
        pm.component('listbox').insert(1,0.0,0.001,0.01)
        makeSpace(self)
#        self.makeCrossoverMenu()
#        makeComboBox(self,"Number of crossover sites",
#                     self.var["crossover sites"])
#        makeComboBox(self,"Swap probability",
#                     self.var["swap probability"])
#        self.makeDissFormMenu()
#        self.makeCombinedFormMenu()
#        makeSpace(self)
#        makeOptionMenu(self,"Reproduction form",["simple",
#                                                 "elitist",
#                                                 "monotonical",
#                                                 "sexuate",
#                                                 "hetero",
#                                                 "hermaphrodite",
#                                                 "diverse"],
#                       self.var["reproduction form"])
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()
        
    def makeCrossoverMenu(self):
        command_str=["1-point","n-point","dissociated","uniform",
                     "combined","all"]
        makeOptionMenu(self,"Crossover form",command_str,
                       self.var["crossover form"])
        
    def makeCombinedFormMenu(self):
        command_str=["performance","balanced","RR50","RR100","adaptive","all"]
        makeOptionMenu(self,"Combination form",command_str,
                       self.var["combined form"])
        
    def makeDissFormMenu(self):
        command_str=["0","and","xor","random","all"]
        makeOptionMenu(self,"Dissociated option",command_str,
                       self.var["diss form"])
        
    def ReConfigure(self):
        return 1

    def quit(self):
        self.top.oper_ctrl=None
        self.destroy()

class RunControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Running options')
        self.var=parent.var
        self.top=parent
        self.si=makeComboBox(self,"Size of the individual",
                        self.var["individual size"])
        self.si.component('listbox').insert(1,10,20,30,50,300,36,45,55,66,78,91,105,122,153,173)
        makeComboBox(self,"Size of the population",self.var["population size"])
        makeComboBox(self,"Number of generations",
                     self.var["generation number"])
        makeSpace(self)
#        makeOptionMenu(self,"Keep the evolution history",["no","yes"],
#                       self.var["history"])
#        self.hist_file_label = Tkinter.Label(self,text="History file:\n %s"%(self.var["history file"].get()),font=get_font())
#        self.hist_file_label.pack()
#        file_browser = Tkinter.Button(self,text="Set history file...",
#                                      font=get_font(),
#                                      command = self.setHistoryFile)
#	file_browser.pack(fill='x')
        self.res_file_label = Tkinter.Label(self,text="Result file:\n %s"%(self.var["result file"].get()),font=get_font())
        self.res_file_label.pack()
        rfile_browser = Tkinter.Button(self,text="Set result file...",
                                       font=get_font(),
                                       command = self.setResultFile)
	rfile_browser.pack(fill='x')
        makeSpace(self)
#        makeComboBox(self,"Number of trials",
#                     self.var["number of trials"])
        makeComboBox(self,"Number of reports",
                     self.var["number of reports"])
        makeOptionMenu(self,"Random seed",["automatic","user determined"],
                       parent.randseed)
        self.rand_box = makeComboBox(self,"random seed",
                                     self.var["random seed"])
        makeSpace(self)
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()

    def setHistoryFile(self):
        fname=tkFileDialog.asksaveasfilename()
        if fname:
            self.var["history file"].set(fname)
            self.hist_file_label.config(text="History file:\n %s"%(self.var["history file"].get()))

    def setResultFile(self):
        fname=tkFileDialog.asksaveasfilename()
        if fname:
            self.var["result file"].set(fname)
            self.res_file_label.config(text="Result file:\n %s"%(self.var["result file"].get()))
        
    def ReConfigure(self):
        self.si.component('entryfield').setentry(self.var["individual size"].get())
        self.rand_box.component('entryfield').setentry(self.var["random seed"].get())

    def quit(self):
        self.top.run_ctrl=None
        self.destroy()
        
class EvaluationControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Evaluation options')
        self.var=parent.var
        self.top=parent
        self.inf_lim=makeComboBox(self,"Inferior limit",self.var["inferior limit"])
        self.inf_lim.component('listbox').insert(1,0,-0.5,-1,-3,-40,-300,-50)
        self.sup_lim=makeComboBox(self,"Superior limit",
                                  self.var["superior limit"])
        self.sup_lim.component('listbox').insert(1,100,1.27,2.047,
                                                 5.11,65.535,511)
        makeSpace(self)
#        makeOptionMenu(self,"Evaluation form",["boolean","real"],
#                       self.var["evaluation form"])
        fnr=makeComboBox(self,"Function number",self.var["function number"])
        fnr.component('listbox').insert(1,1,2,3,4,5,6,7,8,9,10,15)
#        tnr=makeComboBox(self,"Transformation number",
#                     self.var["transformation number"])
#        tnr.component('listbox').insert(1,0,3,4)
#        self.vnr=makeComboBox(self,"Variables number",self.var["var number"])
#        self.vnr.component('listbox').insert(1,1,2,3,5,30)
        makeSpace(self)
#        self.problem_file_label = Tkinter.Label(self,font=get_font(),
#                                                text="Problem file:\n %s"%(self.var["problem file"].get()))
#        self.problem_file_label.pack()
#        file_browser = Tkinter.Button(self,text="Set problem file...",
#                                      font=get_font(),
#                                      command = self.setProblemFile)
#	file_browser.pack(fill='x')
#        makeSpace(self)
#        encode_off=Tkinter.Radiobutton(self, text="Encode off",font=get_font())
#        encode_on=Tkinter.Radiobutton(self, text="Encode on",font=get_font())
#        encode_off["variable"]=self.var["encode"]
#        encode_on["variable"]=self.var["encode"]
#        encode_off["value"]=0
#        encode_on["value"]=1
#        encode_off.pack()
#        encode_on.pack()
#        makeSpace(self)
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()

    def setProblemFile(self):
        fname=tkFileDialog.askopenfilename()
        if fname:
            self.var["problem file"].set(fname)
            self.problem_file_label.config(text="Problem file:\n %s"%(self.var["problem file"].get()))
            self.top.AutoSet()
        
    def ReConfigure(self):
        self.inf_lim.component('entryfield').setentry(self.var["inferior limit"].get())
        self.sup_lim.component('entryfield').setentry(self.var["superior limit"].get())
        self.vnr.component('entryfield').setentry(self.var["var number"].get())

    def quit(self):
        self.top.eval_ctrl=None
        self.destroy()
        
def AppendToName(generic_name, current_value):
    split_name=string.split(generic_name,".")
    length = len(split_name)
    split_name[length-2] = "%s%g" %(split_name[length-2],current_value)
    name=string.join(split_name,".")
    return name

def MakeFilename(generic_name, check_value, current_value):
    check_str="%g" %(check_value)
    split_name=string.split(generic_name,check_str)
    if len(split_name) == 1:
        return AppendToName(generic_name, current_value)
    last_part=split_name[len(split_name) - 1]
    first_list=[]
    for i in range(len(split_name) - 1):
        first_list.append(split_name[i])
    first_part=string.join(first_list, check_str)
    name = string.join([first_part,last_part],"%g" %(current_value))
    return name

def AddToName(old_name, value, new_text):
    split_name=string.split(old_name, "%g" %(value))
    length = len(split_name)
    split_name[length-2] = "%s%s" %(split_name[length-2],new_text)
    name=string.join(split_name,"%g" %(value))
    return name
    
class LoopControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Loop Control')
        self.var=parent.var
        self.top=parent
        
        self.loop_var=Tkinter.StringVar()
        self.loop_var.set("none")
        options_array = self.var.keys()
        options_array.append("none")
        loop_menu = makeStringMenu(self,"Loop over",options_array,
                                   self.loop_var)

        self.start = Tkinter.DoubleVar()
        self.start.set(0)
        self.end = Tkinter.DoubleVar()
        self.end.set(0)
        self.step = Tkinter.DoubleVar()
        self.step.set(0)

        makeComboBox(self,"Starting value",self.start)
        makeComboBox(self,"Ending value",self.end)
        makeComboBox(self,"Step",self.step)

        quit_button=Tkinter.Button(self,text="Run",
                                   font=get_font(),command = self.Run)
	quit_button.pack()

        quit_button=Tkinter.Button(self,text="Quit",
                                   font=get_font(),command = self.Quit)
	quit_button.pack()

    def Run(self):
        if not self.var.has_key(self.loop_var.get()):
            self.top.Run()
        elif self.loop_var.get() == "problem file":
            initial_file = self.var["problem file"].get()
            split_array = os.path.split(initial_file)
            the_path = split_array[0]
            the_file = split_array[1]
            files = os.listdir(the_path)
            Sort(files)
            i=0
            while (i < len(files)) and files[i] != the_file:
                i=i+1
            j=i
            res_file = self.var["result file"].get()
            while (j < len(files)):
                rfilename = "%s.%d" %(res_file, j)
                problem = os.path.join(the_path, files[j])
                self.var["problem file"].set(problem)
                self.var["result file"].set(rfilename)
                self.top.AutoSet()
                self.top.Run_seq()
                j=j+1
        else:
            generic_value = self.var[self.loop_var.get()].get()
            generic_name=self.var["result file"].get()
            value = self.start.get()
            print value
            print self.step.get()
            print self.end.get()
            while value <= self.end.get():
                print self.loop_var.get()
                print self.var[self.loop_var.get()]
                print type(self.var[self.loop_var.get()].get())
                if type(self.var[self.loop_var.get()].get()) is types.IntType:
                    value=int(value)
                print value
                self.var[self.loop_var.get()].set(value)
                name = MakeFilename(generic_name, generic_value, value)
                self.var["result file"].set(name)
                self.top.AutoSet()
                self.top.Run_seq()
                if self.var["reproduction form"].get() == 6:
                    name_rep=AddToName(name, value, "_rep")
                    command="mv ~/temp/gender_stat.txt %s" %(name_rep)
                    os.system(command)
                value = value + self.step.get()

    def Quit(self):
        self.top.loop_ctrl=None
        self.destroy()

if __name__ == '__main__' : 
    ga_ctrl = GAcontrol()
