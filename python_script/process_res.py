#!/usr/bin/env python

# Process results file from the GAD project. Copied in here as an
# example.

import os
import os.path
import string
import math

def Read_graph_file(fname):
    input=open(fname, "r")
    var_dict={}
    line = input.readline()
    numbers = []
    while line:
        if string.find(line, "Total error") == 0:
            line1 = string.split(line)
            nr = string.atof(line1[len(line1)-1])
            numbers.append(nr)
        line = input.readline()
    return numbers

# Old res file, needs work.
def Read_res_file(fname, nr_res):
    input=open(fname, "r")
    line1 = string.split(string.split(input.readline(),'\n')[0],'\t')
    line2 = string.split(input.readline(),'\t')
    var_dict={}
    j=0
    for i in range(len(line1)):
        while not line2[j]:
            j=j+1
        try:
            var_dict[line1[i]] = string.atoi(line2[j])
        except:
            var_dict[line1[i]] = string.atof(line2[j])
        j=j+1
    input.readline()
    line3=string.split(string.split(input.readline(),'\n')[0]," = ")
    try:
        var_dict[line3[0]]=line3[1]
    except:
        print line3
    line=input.readline()
    num_total=[]
    while line:
        line=string.split(line)
        if len(line) > 0:
            numbers=[]
            length = len(line)
            for i in range(nr_res):
                j=length-nr_res+i
                try:
                    numbers.append(string.atoi(line[j]))
                except:
                    numbers.append(string.atof(line[j]))
            num_total.append(numbers)
        line=input.readline()
    return num_total

def Read_res_file_multi(fname, nr_res):
    input=open(fname, "r")
    line1 = string.split(string.split(input.readline(),'\n')[0],'\t')
    line2 = string.split(input.readline(),'\t')
    var_dict={}
    j=0
    for i in range(len(line1)):
        while not line2[j]:
            j=j+1
        try:
            var_dict[line1[i]] = string.atoi(line2[j])
        except:
            var_dict[line1[i]] = string.atof(line2[j])
        j=j+1
    input.readline()
    line3=string.split(string.split(input.readline(),'\n')[0]," = ")
    var_dict[line3[0]]=line3[1]
    which = line3[len(line3)-1]
    dict = {}
    line=input.readline()
    num_total=[]
    while line:
        line=string.split(line)
        if len(line) > 0:
            try:
                string.atof(line[0])   
                numbers=[]
                length = len(line)
                for i in range(nr_res):
                    j=length-nr_res+i
                    try:
                        numbers.append(string.atoi(line[j]))
                    except:
                        try:
                            numbers.append(string.atof(line[j]))
                        except:
                            pass
                if (len(numbers)):
                    num_total.append(numbers)
            except:
                dict[which] = num_total
                which = line[len(line)-1]
                num_total=[]
        line=input.readline()
    dict[which] = num_total
    return dict

def Merge(nums1, nums2):
    for x in nums2:
        nums1.append(x)

def Mean(nums, index):
    mean = 0
    for x in nums:
        try:
            mean = mean+x[index]
        except:
            mean = mean + x
    if (len(nums)):
        return mean/len(nums)
    else:
        return 0
    
def Mean_pos(nums, index):
    mean = 0
    count = 0
    for x in nums:
        if x[index] > 0:
            mean = mean+x[index]
            count = count + 1
    if (len(nums)):
        return mean/count
    else:
        return 0
    
def Mean_thr(nums, index, threshold):
    mean = 0
    count = 0
    for x in nums:
        if x[index] > threshold:
            mean = mean+x[index]
            count = count + 1
    if (len(nums)):
        return mean/count
    else:
        return 0

def Mean_dict(dict, index):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Mean(dict[x], index)
    return dict1

def Mean_dict_pos(dict, index):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Mean_pos(dict[x], index)
    return dict1

def Mean_dict_thr(dict, index, threshold):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Mean_thr(dict[x], index, threshold)
    return dict1

def Mean_dict_multi(dict, how_many):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = []
        for i in range(how_many):
            dict1[x].append(Mean(dict[x], i))
    return dict1

def Plot_data(dict, keys, step, how_many):
    res = []
    gen = 0
    for i in range(how_many):
        res_array=[]
        res_array.append(gen)
        gen=gen+step
        for x in keys:
            res_array.append(dict[x][i])
        res.append(res_array)
    return res

def Max(nums, index):
    try:
        max = nums[0][index]
    except:
        max = nums[0]
    for x in nums:
        try:
            if x[index] > max:
                max = x[index]
        except:
            if x > max:
                max = x
    return max

def Max_dict(dict, index):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Max(dict[x], index)
    return dict1

def Max_dict_multi(dict, how_many):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = []
        for i in range(how_many):
            dict1[x].append(Max(dict[x], i))
    return dict1

def Min(nums, index):
    try:
        min = nums[0][index]
    except:
        min = nums[0]
    for x in nums:
        try:
            if x[index] < min:
                min = x[index]
        except:
            if x < min:
                min = x
    return min

def Min_dict(dict, index):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Min(dict[x], index)
    return dict1

def Stdev(nums, index, ave):
    sum = 0.0
    for x in nums:
        sum = sum + (x[index]-ave) * (x[index]-ave)
    if len(nums):
        sum = sum / len(nums)
    return math.sqrt(sum)

def Stdev_dict(dict, index, ave_dict):
    dict1 = {}
    for x in dict.keys():
        dict1[x] = Stdev(dict[x], index, ave_dict[x])
    return dict1

def Sort(nums):
    n=len(nums)
    for i in range(n-1):
        for j in range(i+1,n):
            if nums[i] > nums[j]:
                temp=nums[i]
                nums[i]=nums[j]
                nums[j] = temp

def Sort_by(nums, index):
    n=len(nums)
    for i in range(n-1):
        for j in range(i+1,n):
            if nums[i][index] > nums[j][index]:
                temp=nums[i]
                nums[i]=nums[j]
                nums[j] = temp

def Mann_whitney(nums1, nums2, index):
    n1 = len(nums1)
    n2 = len(nums2)
    n = n1+n2
    r1 = 0
    r2 = 0
    combined = []
    for x in nums1:
        combined.append([x[index],1])
    for x in nums2:
        combined.append([x[index],2])
    Sort_by(combined, 0)
    i=0
    while i<n:
        j=i
        while j < n and combined[i][0] == combined[j][0]:
            j=j+1
        rank = (i+j-1)/2.0+1
        for k in range(i, j):
            if combined[k][1] == 1:
                r1 = r1 + rank
            else:
                r2 = r2 + rank
        i = j
    u1 = n1 * n2 + n1 * (n1+1)/2 - r1
    u2 = n1 * n2 + n2 * (n2+1)/2 - r2
    if u1>u2:
        u=u1
    else:
        u=u2
    z = (u - n1*n2/2)/math.sqrt(n1*n2*(n1+n2+1)/12)
    return z    

def TTest(list1, list2, index=0):
    ave1 = Mean(list1, index)
    ave2 = Mean(list2, index)
    stdev1=Stdev(list1, index, ave1)
    stdev2=Stdev(list2, index, ave2)
    n1=len(list1)
    n2=len(list2)
    se12=math.sqrt(stdev1*stdev1/n1+stdev2*stdev2/n2)
    t=math.fabs(ave1-ave2)/se12
    print "ave1",ave1,"ave2",ave2,"stdev1",stdev1,"stdev2",stdev2
    print "n1",n1,"n2",n2, "t", t
    # For n1 = n2 = 50, the significance value for a confidence of 0.05
    # is about 1.98. For values bigger than it the difference is significant.
    return t

def Create_file_dict(names, path_name):
    total_dict={}
    file_list=os.listdir(path_name)
    for name in names:
        for file_name in file_list:
            if string.find(file_name, name) == 0:
                abs_file=os.path.join(path_name, file_name)
                if os.path.isdir(abs_file):
                    continue
                number = ""
                i=len(name)
                try:
                    x=file_name[i]
                except:
                    print name, file_name
                    exit
                while ((file_name[i] == '.' and i < len(file_name)-1 and
                        string.find(string.digits, file_name[i+1]) >= 0) or  
                       (string.find(string.digits, file_name[i]) >= 0)):
                    number = number + file_name[i]
                    i = i+1
                if number[len(number)-1] == ".":
                    number[len(number)-1] = '\0'
                if number == "":
                    number = "0"
                if not total_dict.has_key(number):
                    total_dict[number]={}
                if not total_dict[number].has_key(name):
                    total_dict[number][name]=[]
                new_name = os.path.join(path_name, file_name)
                total_dict[number][name].append(new_name)
    return total_dict

def Create_res_dict(file_dict, name_dict):
    res_dict={}
    print file_dict.keys()
    for number in file_dict.keys():
        res_dict[number]={}
        res_by_name={}
        for name in file_dict[number].keys():
            res_array = []
            for fname in file_dict[number][name]:
                res1 = Read_graph_file(fname)
                Merge(res_array, res1)
            res_by_name[name] = res_array
        for name in file_dict[number].keys():
            res_dict[number][name] = []
            res_dict[number][name].append(Mean(res_by_name[name], 0))
            res_dict[number][name].append(Max(res_by_name[name], 0))
            res_dict[number][name].append(Min(res_by_name[name], 0))
            signif = -1
            compare_to = name_dict[name][1]
            if res_by_name.has_key(compare_to):
                signif = Mann_whitney(res_by_name[name],
                                      res_by_name[compare_to], 0)
            res_dict[number][name].append(signif)
    return res_dict

def Write_names_tex(output, names, name_dict):
    name_line = " & "
    for i in range(len(names) - 1):
        name_line = "%s%s & " %(name_line, names[i])
    name_line = "%s%s \\\\" %(name_line, names[len(names) - 1])
    name_line = string.join([name_line, "% "], ' ')
    for name in names:
        if name_dict.has_key(name_dict[name][1]):
            name_line = "%s%s-%s " %(name_line, name, name_dict[name][1])
    output.writelines([name_line,"\n"])

def Write_names_sep(output, names, name_dict, separator):
    name_line = string.join(names, separator)
    name_line = "%s%s%s" %(separator,name_line, separator)
    for name in names:
        if name_dict.has_key(name_dict[name][1]):
            name_line = "%s%s-%s%s" %(name_line, name, name_dict[name][1],
                                      separator)
    output.writelines([name_line,"\n"])

def Write_numbers_tex(output,numbers,names,res_dict,name_dict,which, comment):
    for i in numbers:
        number = "%g" %(i)
        write_line = "%s%s & " %(comment, number)
        print res_dict.keys()
        for i in range(len(names) - 1):
            write_line = "%s%.3f & " %(write_line,
                                       res_dict[number][names[i]][which])
        write_line = "%s%.3f \\\\" %(write_line,
                                     res_dict[number][names[len(names)
                                                            - 1]][which])
        if which == 0:
            write_line = string.join([write_line, "% "], ' ')
            for name in names:
                if name_dict.has_key(name_dict[name][1]):
                    write_line = "%s%.3f " %(write_line,
                                             res_dict[number][name][2])
        output.writelines([write_line, "\n"])

def Write_numbers_sep(output,numbers,names,res_dict,name_dict,which,
                      separator, comment):
    for i in numbers:
        number = "%g" %(i)
        write_line = "%s%s%s" %(comment, number, separator)
        for i in range(len(names) - 1):
            write_line = "%s%f%s" %(write_line,
                                       res_dict[number][names[i]][which],
                                       separator)
        write_line = "%s%f" %(write_line,
                                res_dict[number][names[len(names)- 1]][which])
        if which == 0:
            write_line = "%s%s" %(write_line, separator)
            for name in names:
                if name_dict.has_key(name_dict[name][1]):
                    write_line = "%s%f%s" %(write_line,
                                              res_dict[number][name][2],
                                              separator)
        output.writelines([write_line, "\n"])

def Write_numbers_ave_tex(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_tex(output,numbers,names,res_dict,name_dict,0, comment)

def Write_numbers_max_tex(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_tex(output,numbers,names,res_dict,name_dict,1, comment)

def Write_numbers_min_tex(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_tex(output,numbers,names,res_dict,name_dict,2, comment)
    
def Write_numbers_ave_tabs(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_sep(output,numbers,names,res_dict,name_dict,0,"\t", comment)
    
def Write_numbers_max_tabs(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_sep(output,numbers,names,res_dict,name_dict,1,"\t", comment)
    
def Write_numbers_min_tabs(output,numbers,names,res_dict,name_dict, comment):
    Write_numbers_sep(output,numbers,names,res_dict,name_dict,2,"\t", comment)
    
def Process_results(name_dict, path_name, res_name, comment):
    file_dict = Create_file_dict(name_dict.keys(), path_name)
    res_dict = Create_res_dict(file_dict, name_dict)
    output_name = os.path.join(path_name, res_name)    
    output = open(output_name, "w")
    numbers=[]
    print res_dict.keys()
    for number in res_dict.keys():
        numbers.append(float(number))
    Sort(numbers)
    names=name_dict["order"]
    
    output.write("Average\n")
    Write_names_tex(output, names, name_dict)
    Write_numbers_ave_tex(output,numbers,names,res_dict,name_dict, comment)
    output.write("\n\nMax\n")
    Write_names_tex(output, names, name_dict)
    Write_numbers_max_tex(output,numbers,names,res_dict,name_dict, comment)
    output.write("\n\nMin\n")
    Write_names_tex(output, names, name_dict)
    Write_numbers_min_tex(output,numbers,names,res_dict,name_dict, comment)
    
    output.write("\n\nAverage\n")
    Write_names_sep(output, names, name_dict, "\t")
    Write_numbers_ave_tabs(output,numbers,names,res_dict,name_dict, comment)
    output.write("\n\nMax\n")
    Write_names_sep(output, names, name_dict, "\t")
    Write_numbers_max_tabs(output,numbers,names,res_dict,name_dict, comment)
    output.write("\n\nMin\n")
    Write_names_sep(output, names, name_dict, "\t")
    Write_numbers_min_tabs(output,numbers,names,res_dict,name_dict, comment)
    
    output.close()
    print res_dict
            
def process_file(file_name, nr):
    x= Read_res_file_multi(file_name, nr)
    y=Mean_dict(x, 0)
    print y
    return y

def count_max_val(file_name, nr):
    x = Read_res_file_multi(file_name, nr)
    y = Max_dict(x, 0)
    print "max:"
    print y
    res = {}
    for method in x.keys():
        res[method] = 0
        for i in x[method]:
            if i[0] == y[method]:
                res[method] = res[method]+1
    print res
    return res

def count_stdev(file_name, nr):
    x = Read_res_file_multi(file_name, nr)
    y = Stdev_dict(x, 0)
    print "stdev:"
    print y
    res = {}
    for method in x.keys():
        res[method] = 0
        for i in x[method]:
            if i[0] == y[method]:
                res[method] = res[method]+1
    print res
    return res

def TTest_files(fname1, fname2, nr1, nr2):
    list1=Read_res_file(fname1, nr1)
    list2=Read_res_file(fname2, nr2)
    t = TTest(list1, list2)
    print t
    return t
    # For n1 = n2 = 50, the significance value for a confidence of 0.05
    # is about 1.98. For values bigger than it the difference is significant.
    
def process_ave(filename, nr, name_array):
    y=process_file(filename, nr)
    str = ""
    for i in range(len(name_array)):
        name = name_array[i]
        str = "%s %.3f" %(str, y[name])
    print str
    
if __name__ == '__main__' :
#    dict["gn"] = [4, "hr"]
#    dict["hr"] = [4, ""]
#    dict["sr"] = [3, ""]
#    dict["order"] = ["gn", "hr", "sr"]
#    Process_results(dict, "/home/danav/develop/mc_graph//Results/ga/",
#                    "res_dg.txt", "")
    dict={}
    dict["dg"] = [4, ""]
    dict["order"] = ["dg"]
    Process_results(dict, "../Results/gec05/tv/","tv_res.txt", "")

#    dict={}
#    dict["hc150_m__"] = [3, ""]
#    dict["hc150_ht__"] = [4, ""]
#    dict["hc150_hm__"] = [4, ""]
#    dict["order"] = ["hc150_m__", "hc150_ht__", "hc150_hm__"]
#    Process_results(dict, "/home/danav/develop/GAD/Results/pop_radius_unsorted/",
#                    "result_u.txt", "")
#    x=Read_res_file("/home/danav/develop/GAD/Results/psize/gn100.txt",4)
#    y=Read_res_file("/home/danav/develop/GAD/Results/psize/gn100m.txt",4)
#    print "Mean x: ", Mean(x, 0)
#    print "Max  x: ", Max(x, 0)
#    print "Min  x: ", Min(x, 0)
#    print "Mean y: ", Mean(y, 0)
#    print "Max  y: ", Max(y, 0)
#    print "Min  y: ", Min(y, 0)
#    print "M-W xy: ", Mann_whitney(x, y, 0)
# from process_res import *
# x= Read_res_file_multi("/home/danav/develop/GAD/Results/gecco03/hc50res.txt.10", 3)
# y=Mean_dict(x, 0)
#
# x= Read_res_file_multi("/home/danav/develop/GAD/Results/gecco03/hc50res.txt.10", 23)
# y = Mean_dict_multi(x, 21)
# data=Plot_data(y, ['2-points','and','balanced','adaptive'],50,21)
# count_max_val("/home/danav/develop/GAD/Results/gecco03/f0res1.txt",3)

# x=['1-point','2-points','0.50','and']
# process_ave("/home/danav/develop/GAD/Results/gecco03/f1res1.txt",3,x)

# T-Test for 2 files:
#TTest_files("../Results/gender_100/gend_hc/gend_hc100.txt","../Results/gender_100/gend_hc/gend_m_hc100.txt", 4, 4)
