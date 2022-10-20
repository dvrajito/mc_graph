#!/usr/bin/env python

# Read the result files, extract the data, compute an average, and
# output in a tex format and a tab-delimited, spreadsheet-usable
# format.

import os
import os.path
import string
import math

# Used by Read_res_file
def Get_line(input):
    line = input.readline()
    if line:
        line = string.split(line,'\n')[0]
    return line

# Used by Read_res_file
def Set_def_vals(dict, key):
    if not dict.has_key(key):
        dict[key]=[]
    dict_new = {}
    dict_new={}
    dict_new["error"]={}
    dict_new["error"]["ave"] = 0
    dict_new["error"]["max"] = -1
    dict_new["error"]["min"] = -1
    dict_new["error"]["stdev"] = -1
    dict_new["error"]["start"] = -1
    dict_new["norm"]={}
    dict_new["norm"]["ave"] = 0
    dict_new["norm"]["max"] = -1
    dict_new["norm"]["min"] = -1
    dict_new["norm"]["stdev"] = -1
    dict_new["method"] = -1
    dict_new["iterations"] = -1
    dict_new["epsilon"] = -1
    dict_new["edges"] = -1
    dict_new["vertices"] = -1
    dict_new["stop_iter"] = -1
    dict_new["total_weight"] = -1
    dict[key].append(dict_new)

# Used by Read_res_file
def Close_trials(dict, key, iterations, errors, norms):
    the_dict=dict[key][len(dict[key])-1]
    if iterations > 0:
        the_dict["error"]["ave"] = the_dict["error"]["ave"]/iterations
        the_dict["error"]["start"] = the_dict["error"]["start"]/iterations
        the_dict["norm"]["ave"] = the_dict["norm"]["ave"]/iterations
        if the_dict["stop_iter"] != -1:
            the_dict["stop_iter"] = the_dict["stop_iter"]/iterations
        if len(errors):
            stdev=0
            ave = the_dict["error"]["ave"]
            for x in errors:
                stdev = stdev + (x-ave)*(x-ave)
            stdev = stdev / iterations
            the_dict["error"]["stdev"]=stdev
            errors = []
        if len(norms):
            stdev=0
            ave = the_dict["norm"]["ave"]
            for x in norms:
                stdev = stdev + (x-ave)*(x-ave)
            stdev = stdev / iterations
            the_dict["norm"]["stdev"]=stdev
            norms = []
        if the_dict.has_key("time"):
            the_dict["time"] = the_dict["time"]/iterations
    the_dict["trials"] = iterations

# Updated July 23, 2004. Modified to read in the new kind of data we
# write to the result files.
def Read_res_file(fname):
    input=open(fname, "r")
    problem=""
    graph = ""
    iters=0
    eof = 0
    results = {}
    nr_res = 0
    context = ""
    errors = []
    norms = []
    old_pos = -1
    quit = 0
    index=-1
    while not quit:
        line = Get_line(input)
        file_pos = input.tell()
        if (file_pos == old_pos):
            quit = 1
        else:
            old_pos = file_pos
        if string.find(line, "Graph file:") != -1:
            if index >= 0 and graph != "":
                Close_trials(results, graph, iterations, errors, norms)
            problem=string.split(line, "Graph file: ")[1]
            graph = os.path.basename(problem)
            Set_def_vals(results, graph)
            iterations = 0
            index = len(results[graph])-1
        elif string.find(line, "iterations:") > 1:
            stopit = int(string.split(line,"iterations: ")[1])
            if stopit == -1 and results[graph][index]["iterations"] > 0:
                stopit = results[graph][index]["iterations"]
            if results[graph][index]["stop_iter"] == -1:
                results[graph][index]["stop_iter"] = stopit
            else:
                stopit = stopit + results[graph][index]["stop_iter"]
                results[graph][index]["stop_iter"] = stopit
        elif string.find(line, "method:") != -1:
            method = int(string.split(line,"method: ")[1])
            if results[graph][index]["method"] == -1:
                results[graph][index]["method"] = method
            elif results[graph][index]["method"] != method:
                Close_trials(results, graph, iterations, errors, norms)
                iterations = 0
                index = index + 1
                Set_def_vals(results, graph)
            iterations = iterations + 1
        elif string.find(line, "iterations:") != -1 and string.find(line, "Diverge iterations:") == -1:
            iters = int(string.split(line,"iterations: ")[1])
            if results[graph][index]["iterations"] == -1:
                results[graph][index]["iterations"] = iters
            elif results[graph][index]["iterations"] != iters:
                Close_trials(results, graph, iterations, errors, norms)
                iterations = 0
                index = index + 1
                Set_def_vals(results, graph)
        elif string.find(line, "epsilon:") != -1:
            epsilon = float(string.split(line,"epsilon: ")[1])
            if results[graph][index]["epsilon"] == -1:
                results[graph][index]["epsilon"] = epsilon
            elif results[graph][index]["epsilon"] != epsilon:
                Close_trials(results, graph, iterations, errors, norms)
                iterations = 0
                index = index + 1
                Set_def_vals(results, graph)
        elif string.find(line, "vertices:") != -1:
            if results[graph][index]["vertices"] == -1:
                results[graph][index]["vertices"] = int(string.split(line,"vertices: ")[1])
        elif string.find(line, "edges:") != -1:
            if results[graph][index]["edges"] == -1:
                results[graph][index]["edges"] = int(string.split(line,"edges: ")[1])
        elif string.find(line, "total edge weight:") != -1:
            tw = float(string.split(line,"total edge weight: ")[1])
            if results[graph][index]["total_weight"] == -1:
                results[graph][index]["total_weight"] = tw
        elif string.find(line, "total error:") != -1:
            serr = float(string.split(line,"total error: ")[1])
            results[graph][index]["error"]["start"]=results[graph][index]["error"]["start"]+serr
            context = "error"
        elif string.find(line, "Total time:") != -1:
            Close_trials(results, graph, iterations, errors, norms)
            ttm = float(string.split(line,"Total time: ")[1])
            results[graph][index]["time"] = ttm
            graph=""
        elif string.find(line, "total tension vector norm:") != -1:
            tvn = float(string.split(line,"total tension vector norm: ")[1])
            norms.append(tvn)
            results[graph][index]["norm"]["ave"] = results[graph][index]["norm"]["ave"]+tvn
            if (results[graph][index]["norm"]["min"] == -1 or
                results[graph][index]["norm"]["min"] < tvn):
                results[graph][index]["norm"]["min"] = tvn
            if (results[graph][index]["norm"]["max"] == -1 or
                results[graph][index]["norm"]["max"] > tvn):
                results[graph][index]["norm"]["max"] = tvn
        elif string.find(line, "layout:") != -1:
            context = "layout"
        else:
            numbers = string.split(line)
            if context == "error":
                if len(numbers):
                    last_err = float(numbers[len(numbers)-1])
                    errors.append(last_err)
                    results[graph][index]["error"]["ave"] = results[graph][index]["error"]["ave"] + last_err
                    if (results[graph][index]["error"]["min"] == -1 or
                        results[graph][index]["error"]["min"] < last_err):
                        results[graph][index]["error"]["min"] = last_err
                    if (results[graph][index]["error"]["max"] == -1 or
                        results[graph][index]["error"]["max"] > last_err):
                        results[graph][index]["error"]["max"] = last_err
    if graph != "":
        Close_trials(results, graph, iterations, errors, norms)
    input.close()
    return results

def Merge(nums1, nums2):
    for x in nums2:
        nums1.append(x)

def Mean(nums, index):
    mean = 0
    for x in nums:
        mean = mean+x[index]
    return mean/len(nums)

def Max(nums, index):
    max = nums[0][index]
    for x in nums:
        if x[index] > max:
            max = x[index]
    return max

def Min(nums, index):
    min = nums[0][index]
    for x in nums:
        if x[index] < min:
            min = x[index]
    return min

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

# Useful if there is just one number in the name.
def Create_file_dict(name, path_name):
    total_dict={}
    file_list=os.listdir(path_name)
    for file_name in file_list:
        if string.find(file_name, name) == 0:
            number = ""
            i=len(name)
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
                total_dict[number]=[]
            new_name = os.path.join(path_name, file_name)
            total_dict[number].append(new_name)
    return total_dict

def Create_res_dict(file_dict, name_array, nr_iter):
    res_dict={}
    #print file_dict.keys()
    for number in file_dict.keys():
        res_array = {}
        for fname in file_dict[number]:
            split_name = string.split(fname, '/')
            last_name = split_name[len(split_name) - 1]
            for suffix in name_array:
                if string.find(last_name, suffix) >= 0:
                    result = Read_res_file(fname, nr_iter)
                    if result['nr_res'] != 0:
                        res_array[suffix]=result
        if res_array:
            res_dict[number] = res_array
    return res_dict

def Write_names_tex(output, names_array):
    name_line = " & "
    for i in range(len(names_array)):
        name_line = "%s%s & " %(name_line, names_array[i])
    name_line = "%s total tvector norm \\\\" %(name_line)
    output.writelines([name_line,"\n"])

def Write_names_sep(output, names, separator):
    name_line = string.join(names, separator)
    name_line = "%s%s%s" %(separator, name_line, separator)
    output.writelines([name_line,"\n"])

def Write_numbers_cols_tex(output,numbers,names,res_dict,which,comment):
    for i in numbers:
        number = "%g" %(i)
        write_line = "%s%s & " %(comment, number)
        for name in names:
            if res_dict[number].has_key(name):
                length = len(res_dict[number][name][which])
                try:
                    write_line = "%s%.3f & " %(write_line,
                                               res_dict[number][name][which][length-1])
                except:
                    print number, name, which, length
        if (res_dict[number].has_key('tv')):
            write_line = "%s%.3f \\\\" %(write_line,
                                         res_dict[number]['tv']['total_norm'][which])
        else:
            write_line = "%s \\\\" %(write_line)
        output.writelines([write_line, "\n"])

def Write_numbers_sep_col(output,numbers,names,res_dict,which,
                          separator, comment):
    for i in numbers:
        number = "%g" %(i)
        write_line = "%s%s%s" %(comment, number, separator)
        for name in names:
            if res_dict[number].has_key(name):
                length = len(res_dict[number][name][which])
                write_line = "%s%.3f%s" %(write_line,
                                          res_dict[number][name][which][length-1],
                                          separator)
            
        if (res_dict[number].has_key('tv')):
            write_line = "%s%.3f" %(write_line,
                                    res_dict[number]['tv']['total_norm'][which])
        output.writelines([write_line, "\n"])
        
def Write_numbers_sep_nr(output,numbers,names,res_dict,which,
                          separator, comment, how_many):
    for nr in numbers:
        number = "%g" %(nr)
        output.writelines(['\n', number, '\n'])
        for name in names:
            if res_dict[number].has_key(name):
                write_line = "%s%s%s%s%.3f%s" %(comment, number, name, separator,
                                              res_dict[number][name][which][0], separator)
                length = len(res_dict[number][name][which])
                step = float(length-1)/how_many
                for i in range(how_many):
                    ind = int((i+1)*step)
                    res = res_dict[number][name][which][ind]
                    write_line = "%s%.3f%s" %(write_line, res, separator)
            
                if (name == 'tv'):
                    write_line = "%s%.3f" %(write_line,
                                            res_dict[number]['tv']['total_norm'][which])
                output.writelines([write_line, "\n"])
        
def Write_numbers_sep_name(output,numbers,names,res_dict,which,
                           separator, comment, how_many):
    for name in names:
        output.writelines(['\n', name, '\n'])
        for nr in numbers:
            number = "%g" %(nr)
            if res_dict[number].has_key(name):
                write_line = "%s%s%s%.3f%s" %(comment, number, separator,
                                              res_dict[number][name][which][0],
                                              separator)
                length = len(res_dict[number][name][which])
                step = float(length-1)/how_many
                for i in range(how_many):
                    ind = int((i+1)*step)
                    res = res_dict[number][name][which][ind]
                    write_line = "%s%.3f%s" %(write_line, res, separator)
            
                if (name == 'tv'):
                    write_line = "%s%.3f" %(write_line,
                                            res_dict[number]['tv']['total_norm'][which])
                output.writelines([write_line, "\n"])

# Comparison by method then by iterations for the sort.
def compare_meth(dict1, dict2):
    if dict1["iterations"] > dict2["iterations"]:
        return 1
    elif dict1["iterations"] == dict2["iterations"]:
        if dict1["method"] > dict2["method"]:
            return 1
        elif dict1["method"] == dict2["method"]:
            return 0
        else:
            return -1
    else:
        return -1

# Comparison by epsilon then by iterations for the sort.
def compare_eps(dict1, dict2):
    if dict1["iterations"] > dict2["iterations"]:
        return 1
    elif dict1["iterations"] == dict2["iterations"]:
        if dict1["epsilon"] > dict2["epsilon"]:
            return 1
        elif dict1["epsilon"] == dict2["epsilon"]:
            return 0
        else:
            return -1
    else:
        return -1

# Comparison by method and epsilon.
def compare_eps_meth(dict1, dict2):
    if dict1["epsilon"] > dict2["epsilon"]:
        return 1
    elif dict1["epsilon"] == dict2["epsilon"]:
        if dict1["method"] > dict2["method"]:
            return 1
        elif dict1["method"] == dict2["method"]:
            return 0
        else:
            return -1
    else:
        return -1

# Group the results by epsilon: a different table for each value of
# epsilon.
def Group_epsilon(res_dict):
    results = {}
    graphs=[]
    for graph in res_dict.keys():
        graphs.append(graph)
    graphs.sort()
    for graph in graphs:
        for dict in res_dict[graph]:
            eps = dict["epsilon"]
            if not results.has_key(eps):
                results[eps]={}
            if not results[eps].has_key(graph):
                results[eps][graph]=[]
            results[eps][graph].append(dict)
    for eps in results.keys():
        for graph in results[eps].keys():
            results[eps][graph].sort(compare_meth)
    return results

# Group the results by method: a different table for each method.
def Group_method(res_dict):
    results = {}
    graphs=[]
    for graph in res_dict.keys():
        graphs.append(graph)
    graphs.sort()
    for graph in graphs:
        for dict in res_dict[graph]:
            meth = dict["method"]
            if not results.has_key(meth):
                results[meth]={}
            if not results[meth].has_key(graph):
                results[meth][graph]=[]
            results[meth][graph].append(dict)
    for meth in results.keys():
        for graph in results[meth].keys():
            results[meth][graph].sort(compare_eps)
    return results

# Group the results by iterations: a different table for each number
# of iterations.
def Group_iters(res_dict):
    results = {}
    graphs=[]
    for graph in res_dict.keys():
        graphs.append(graph)
    graphs.sort()
    for graph in graphs:
        for dict in res_dict[graph]:
            iter = dict["iterations"]
            if not results.has_key(iter):
                results[iter]={}
            if not results[iter].has_key(graph):
                results[iter][graph]=[]
            results[iter][graph].append(dict)
    for iter in results.keys():
        for graph in results[iter].keys():
            results[iter][graph].sort(compare_eps_meth)
    return results

# Merge all the results from different files into one dictionary with
# key defined as names of graph files.
# Used by Process_results(...).
def Group_results(file_list, path_name):
    res_dict ={}
    for file_name in file_list:
        abs_file=os.path.join(path_name, file_name)
        results = Read_res_file(abs_file)
        for graph in results.keys():
            if not res_dict.has_key(graph):
                res_dict[graph]=[]
            for x in results[graph]:
                res_dict[graph].append(x)
    return res_dict

# Find all the values that are constant in a given array of
# dictionaries.
def Find_constant(dict_array):
    consts={}
    # -1 for unknown, -2 for not constant.
    consts["epsilon"] = -1
    consts["method"] = -1
    consts["iterations"] = -1
    consts["trials"] = -1
    for dict in dict_array:
        for key in dict.keys():
            if consts.has_key(key):
                if consts[key] == -1:
                    consts[key] = dict[key]
                elif consts[key] != -2:
                    if consts[key] != dict[key]:
                        consts[key] = -2
    return consts

# Find all the values that are constant in a dictionary of arrays of
# dictionaries.
def Find_constant_dict(dicts):
    consts={}
    # -1 for unknown, -2 for not constant.
    consts["epsilon"] = -1
    consts["method"] = -1
    consts["iterations"] = -1
    consts["trials"] = -1
    try:
        for key in dicts.keys():
            dict_array = dicts[key]
            for dict in dict_array:
                for key in dict.keys():
                    if consts.has_key(key):
                        if consts[key] == -1:
                            consts[key] = dict[key]
                        elif consts[key] != -2:
                            if consts[key] != dict[key]:
                                consts[key] = -2
    except:
        for dict in dicts:
            for key in dict.keys():
                if consts.has_key(key):
                    if consts[key] == -1:
                        consts[key] = dict[key]
                    elif consts[key] != -2:
                        if consts[key] != dict[key]:
                            consts[key] = -2
    return consts

# Write a message based on the criteria.
def Write_criteria(output, criteria):
    if criteria == "e":
        output.write("epsilon")
    elif criteria == "i":
        output.write("iterations")
    elif criteria == "m":
        output.write("method")
    else:
        output.write("graph")

def Write_first_consts(output, first, consts, criteria):
    output.write("Table for ")
    Write_criteria(output, criteria)
    output.write(" ")
    if type(first) == type(""):
        output.write(first)
    elif type(first) == type(1):
        output.write("%d" %(first))
    else:
        output.write("%g" %(first))
    output.write("\n")
    if consts["epsilon"] >= 0:
        output.write("epsilon:\t%g\n" %(consts["epsilon"]))
    if consts["method"]>= 0:
        output.write("method:\t%d\n" %(consts["method"]))
    if consts["iterations"] >= 0:
        output.write("iterations:\t%d\n" %(consts["iterations"]))
    if consts["trials"] >= 0:
        output.write("trials:\t%d\n" %(consts["trials"]))

# Write a particular key from an array of dictionaries with given
# separators between the numbers and at the end of the line. The
# second parameter is a dictionary containing all the keywords that
# are constant in this array. The goal is to write it as two tables:
# one for the error, one for the norm.
def Write_key_dict_array(output, dict_array, consts, sep_int, sep_end,
                         key, text):
    iter = None
    if consts["iterations"] < 0:
        iter = 1
    # Output the error for all methods
    output.write("%s\n"%(text))
    output.write("epsilon%s" %(sep_int))
    if iter:
        output.write("iterations%s" %(sep_int))
    output.write("bf%sr%stv%srv" %(sep_int, sep_int, sep_int))
    iters = -1
    eps = -1
    method = 0
    for dict in dict_array:
        if iters != dict["iterations"] or eps != dict["epsilon"]:
            iters = dict["iterations"]
            eps = dict["epsilon"]
            method = 0
            output.write("%s\n%g%s" %(sep_end, eps, sep_int))
            if iter:
                output.write("%d%s" %(iters, sep_int))
        while method < dict["method"]:
            output.write(" %s" %(sep_int))
            method = method + 1
        output.write("%g" %(dict[key]))
        if dict["method"] != 3:
            output.write("%s" %(sep_int))
        method = method + 1
        if method > 3:
            method = 0
    output.write("%s\n\n" %(sep_end))

# Write the time from an an array of dictionaries with given
# separators between the numbers and at the end of the line. The
# second parameter is a dictionary containing all the keywords that
# are constant in this array. The goal is to write it as two tables:
# one for the error, one for the norm.
def Write_time_dict_array(output, dict_array, consts, sep_int, sep_end):
    Write_key_dict_array(output, dict_array, consts, sep_int, sep_end,
                         "time", "Average time")

# Write an array of dictionaries with given separators between the
# numbers and at the end of the line. The second parameter is a
# dictionary containing all the keywords that are constant in this
# array. The goal is to write it as two tables: one for the error, one
# for the norm.
def Write_dict_array(output, dict_array, consts, keyword, sep_int, sep_end):
    iter = None
    if consts["iterations"] < 0:
        iter = 1
    # Output the error for all methods
    output.write("%s error\n" %(keyword))
    output.write("epsilon%s" %(sep_int))
    if iter:
        output.write("iterations%s" %(sep_int))
    output.write("bf%sr%stv%srv" %(sep_int, sep_int, sep_int))
    iters = -1
    eps = -1
    method = 0
    for dict in dict_array:
        if iters != dict["iterations"] or eps != dict["epsilon"]:
            iters = dict["iterations"]
            eps = dict["epsilon"]
            method = 0
            output.write("%s\n%g%s" %(sep_end, eps, sep_int))
            if iter:
                output.write("%d%s" %(iters, sep_int))
        while method < dict["method"]:
            output.write(" %s" %(sep_int))
            method = method + 1
        output.write("%g" %(dict["error"][keyword]))
        if dict["method"] != 3:
            output.write("%s" %(sep_int))
        method = method + 1
        if method > 3:
            method = 0
    output.write("%s\n\n" %(sep_end))

    # Output the norm for the last two methods.
    if consts["method"]<0 or consts["method"] > 1:
        output.write("%s total norm\n" %(keyword))
        output.write("epsilon%s" %(sep_int))
        if iter:
            output.write("iterations%s" %(sep_int))
        output.write("tv%srv" %(sep_int))
        iters = -1
        eps = -1
        for dict in dict_array:
            if iters != dict["iterations"] or eps != dict["epsilon"]:
                iters = dict["iterations"]
                eps = dict["epsilon"]
                output.write("%s\n%g%s" %(sep_end, eps, sep_int))
                if iter:
                    output.write("%d%s" %(iters, sep_int))
            if dict["method"] == 2 or dict["method"] == 3:
                output.write("%g" %(dict["norm"][keyword]))
            if dict["method"] == 2:
                output.write("%s" %(sep_int))
        output.write("%s\n\n" %(sep_end))

# Extract all values of a particular key in a dictionary of arrays of
# dictionaries.
def extract_key(results, keyword):
    values=[]
    for x in results.keys():
        for dict in results[x]:
            if values.count(dict[keyword]) == 0:
                values.append(dict[keyword])
    values.sort()
    return values

# Write a particular key from the dictionary into the result file.
def Write_key_dict(output, results, consts, sep_int, sep_end, key, text):
    iter = None
    if consts["iterations"] < 0:
        iter = 1
    key_row = ""
    values = []
    if consts["method"] == -2:
        key_row = "method"
        values = [0, 1, 2, 3]
    elif consts["epsilon"] == -2:
        key_row = "epsilon"
        values=extract_key(results, "epsilon")
    elif consts["iterations"] == -2:
        key_row = "iterations"
        values = extract_key(results, "iterations")
    # Output the error for all methods
    output.write("%s\n" %(text))
    output.write("Graph%s" %(sep_int))
    if key_row != "iterations" and iter:
        output.write("iterations%s" %(sep_int))
    if key_row == "method":
        output.write("bf%sr%stv%srv" %(sep_int, sep_int, sep_int))
    elif key_row != "":
        output.write("%s%s\n%s" %(key_row, sep_end, sep_int))
        for i in range(len(values)):
            output.write("%g" %(values[i]))
            if i<len(values)-1:
                output.write("%s" %(sep_int))
    iters = -1
    val_ind = 0
    graphs = []
    try:
        for graph in results.keys():
            graphs.append(graph)
    except:
        graphs = graphs + results
    graphs.sort()
    for graph in graphs:
        try:
            dict_array = results[graph]
        except:
            dict_array = [graph]
        for dict in dict_array:
            if iters != dict["iterations"] or val_ind >= len(values):
                iters = dict["iterations"]
                output.write("%s\n%s%s" %(sep_end, graph, sep_int))
                val_ind = 0
            if iter:
                output.write("%d%s" %(iters, sep_int))
            if key_row != "":
                while values[val_ind] < dict[key_row]:
                    output.write(" %s" %(sep_int))
                    val_ind = val_ind + 1
            if dict.has_key(key):
                output.write("%g" %(dict[key]))
            else:
                output.write("na")
            if val_ind < len(values) - 1:
                output.write("%s" %(sep_int))
            val_ind = val_ind + 1
    output.write("%s\n\n" %(sep_end))
    
# Writes the times in a dictionary of arrays of dictionaries with
# given separators between the numbers and at the end of the line. The
# second parameter is a dictionary containing all the keywords that
# are constant in this array. The goal is to write it as two tables:
# one for the error, one for the norm.
def Write_time_dict(output, results, consts, sep_int, sep_end):
    Write_key_dict(output, results, consts, sep_int, sep_end,
                   "time", "Average time")
                
# Write a dictionary of arrays of dictionaries with given separators
# between the numbers and at the end of the line. The second parameter
# is a dictionary containing all the keywords that are constant in
# this array. The goal is to write it as two tables: one for the
# error, one for the norm.
def Write_dict(output, results, consts, keyword, sep_int, sep_end):
    iter = None
    if consts["iterations"] < 0:
        iter = 1
    key_row = ""
    values = []
    if consts["method"] == -2:
        key_row = "method"
        values = [0, 1, 2, 3]
    elif consts["epsilon"] == -2:
        key_row = "epsilon"
        values=extract_key(results, "epsilon")
    elif consts["iterations"] == -2:
        key_row = "iterations"
        values = extract_key(results, "iterations")
    # Output the error for all methods
    output.write("%s error\n" %(keyword))
    output.write("Graph%s" %(sep_int))
    if key_row != "iterations" and iter:
        output.write("iterations%s" %(sep_int))
    if key_row == "method":
        if consts["epsilon"] == -2:
            output.write("epsilon%s" %(sep_int))
        output.write("bf%sr%stv%srv" %(sep_int, sep_int, sep_int))
    elif key_row != "":
        output.write("%s%s\n%s" %(key_row, sep_end, sep_int))
        for i in range(len(values)):
            output.write("%g" %(values[i]))
            if i<len(values)-1:
                output.write("%s" %(sep_int))
    iters = -1
    val_ind = 0
    graphs = []
    try:
        for graph in results.keys():
            graphs.append(graph)
    except:
        pass
    graphs.sort()
    for graph in graphs:
        output.write("%s\n%s%s" %(sep_end, graph, sep_int))
        if consts["epsilon"] == -2 and key_row != "epsilon":
            output.write("%g%s" %(dict["epsilon"], sep_int))
        dict_array = results[graph]
        for val_ind in range(len(values)):
            dict = None
            for d in dict_array:
                if d[key_row] == values[val_ind]:
                    dict = d
            if dict:
                output.write("%g" %(dict["error"][keyword]))
            else:
                output.write(" ")
            if val_ind < len(values) - 1:
                output.write("%s" %(sep_int))
    output.write("%s\n\n" %(sep_end))

    # Output the norm for the last two methods.
    if consts["method"] == -2 or consts["method"] > 1:
        if key_row == "method":
            values=[2, 3]
        output.write("%s total norm\n" %(keyword))
        output.write("Graph%s" %(sep_int))
        if key_row != "iterations" and iter:
            output.write("iterations%s" %(sep_int))
        if key_row == "method":
            if consts["epsilon"] == -2:
                output.write("epsilon%s" %(sep_int))
            output.write("tv%srv" %(sep_int))
        elif key_row != "":
            output.write("%s%s\n%s" %(key_row, sep_end, sep_int))
            for i in range(len(values)):
                output.write("%g" %(values[i]))
                if i<len(values)-1:
                    output.write("%s" %(sep_int))
        for graph in graphs:
            dict_array = results[graph]
            output.write("%s\n%s%s" %(sep_end, graph, sep_int))
            if consts["epsilon"] == -2 and key_row != "epsilon":
                output.write("%g%s" %(dict_array[0]["epsilon"], sep_int))
            iters = dict_array[0]["iterations"]
            val_ind = 0
            for dict in dict_array:
                if iters != dict["iterations"] or val_ind >= len(values):
                    iters = dict["iterations"]
                    output.write("%s\n%s%s" %(sep_end, graph, sep_int))
                    if consts["epsilon"] == -2 and key_row != "epsilon":
                        output.write("%g%s" %(dict["epsilon"], sep_int))
                    val_ind = 0
                if iter:
                    output.write("%d%s" %(iters, sep_int))
                if dict["method"] >= 2 or consts["method"] >=2:
                    if key_row != "":
                        while values[val_ind] < dict[key_row]:
                            output.write(" %s" %(sep_int))
                            val_ind = val_ind + 1   
                    output.write("%g" %(dict["norm"][keyword]))
                    val_ind = val_ind + 1
                    if val_ind != len(values):
                        output.write("%s" %(sep_int))
        output.write("%s\n\n" %(sep_end))

# Write the info on the graphs: number of veritces, of edges, and
# total weight for each of them.
def Write_graph_info(output, results, sep_int, sep_end):
    graphs = []
    for graph in results.keys():
        graphs.append(graph)
    graphs.sort()
    output.write("Information on the graphs\n")
    output.write("Graph%sVertices%sEdges%sTotal Weight%s\n" %(sep_int, sep_int,
                                                             sep_int, sep_end))
    for graph in graphs:
        text = "%s%s%d%s%d%s%g%s\n" %(graph, sep_int,
                                      results[graph][0]["vertices"], sep_int,
                                      results[graph][0]["edges"], sep_int,
                                      results[graph][0]["total_weight"],
                                      sep_end)
        output.write(text)
    output.write("\n")

# Write an array of dictionaries in tex format.
def Write_tex_dict_array(output, dict_array, consts):
    Write_dict_array(output, dict_array, consts, "ave", " & ", "\\\\")

# Write a dictionary of arrays of dictionaries in tex format.
def Write_tex_dict(output, dict, consts):
    Write_dict(output, dict, consts, "ave", " & ", "\\\\")

# Write the results in tex format by a criteria.
def Write_tex(output, results, criteria):
    output.write("Tex format, average\n\n")
    numbers=[]    
    for first in results.keys():
        numbers.append(first)
    numbers.sort()
    for first in numbers:
        if criteria == "g":
            consts = Find_constant(results[first])
        else:
            consts = Find_constant_dict(results[first])
        Write_first_consts(output, first, consts, criteria)
        if criteria == "g":
            Write_tex_dict_array(output, results[first], consts)
        else:
            Write_tex_dict(output, results[first], consts)

# Write the results in tex format by a criteria.
def Write_tab(output, results, criteria):
    output.write("Tab separated format, average\n\n")
    numbers=[]    
    for first in results.keys():
        numbers.append(first)
    numbers.sort()
    for first in numbers:
        if criteria == "g":
            consts = Find_constant(results[first])
        else:
            consts = Find_constant_dict(results[first])
        Write_first_consts(output, first, consts, criteria)
        for keyword in ("ave", "min", "max"):
            if criteria == "g":
                Write_dict_array(output, results[first],
                                 consts, keyword, "\t", "")
            else:
                Write_dict(output, results[first], consts, keyword, "\t", "")
        if criteria == "g":
            Write_time_dict_array(output, results[first], consts, "\t", "")
            Write_key_dict_array(output, results[first], consts, "\t", "",
                                 "stop_iter", "Average stop iteration")
        else:
            Write_time_dict(output, results[first], consts, "\t", "")
            Write_key_dict(output, results[first], consts, "\t", "",
                           "stop_iter", "Average stop iteration")

# Main function to be used. Started updating this July 23, 2004.
# name - A pattern to be found in result files to be processed.
#        Example: if name is "dg", the function will process al the files
#        starting with "dg".
# path_name - The path inside which the function should look for
#             result files.
# res_name  - name of the result file to output the synthesis done by
#             this function.
# criteria  - value by which the results should be displayed
#             "e" for epsilon
#             "m" for method
#             "i" for number of iterations
#             "g" for the graph   
def Process_results(name, path_name, res_name, criteria):
    file_list=[]
    files=os.listdir(path_name)
    for file_name in files:
        if string.find(file_name, name) == 0:
            file_list.append(file_name)
    file_list.sort()
    res_dict = Group_results(file_list, path_name)
    output_name = os.path.join(path_name, res_name)    
    output = open(output_name, "w")
    Write_graph_info(output, res_dict, " & ", "\\\\")
    Write_graph_info(output, res_dict, "\t", "")

    if criteria == "e":
        results=Group_epsilon(res_dict)
    elif criteria == "m":
        results=Group_method(res_dict)
    elif criteria == "i":
        results=Group_iters(res_dict)
    else:
        results=res_dict
        for g in results.keys():
            dict=results[g]
            dict.sort(compare_eps_meth)

    output.write("Tex format\n\nAverage\n")
    Write_tex(output, results, criteria)
    output.write("Tab separated format\n\nAverage, min, max\n")
    Write_tab(output, results, criteria)
    output.close()

def Cut_array(array, nr_iter, limit):
    length = len(array)
    step = nr_iter / (length-1)
    new_nr = limit / step + 1
    new_array = []
    for i in range(new_nr):
        new_array.append(array[i])
    return new_array
    
def Select(res_dict, nr_iter, limit):
    #print res_dict['60']['tv']['total_norm']
    for number in res_dict.keys():
        for method in res_dict[number].keys():
            if res_dict[number][method].has_key('ave'):
                array = res_dict[number][method]['ave']
                res_dict[number][method]['ave'] = Cut_array(array,
                                                            nr_iter, limit)
            if res_dict[number][method].has_key('min'):
                array = res_dict[number][method]['min']
                res_dict[number][method]['min'] = Cut_array(array,
                                                            nr_iter, limit)
            if res_dict[number][method].has_key('max'):
                array = res_dict[number][method]['max']
                res_dict[number][method]['max'] = Cut_array(array,
                                                            nr_iter, limit)

# To be rewritten with the following parameters:
# def Process_results_limit(name, path_name, res_name, criteria, limit):
def Process_results_limit(name, name_array, path_name, res_name, comment,
                          nr_iter, how_many, limit):
    file_dict = Create_file_dict(name, path_name)
    res_dict = Create_res_dict(file_dict, name_array, nr_iter)
    output_name = os.path.join(path_name, res_name)    
    output = open(output_name, "w")
    numbers=[]
    #print res_dict.keys()
    for number in res_dict.keys():
        numbers.append(float(number))
    Sort(numbers)
    Select(res_dict, nr_iter, limit)

    #tex
    output.write("Tex format\n\nAverage\n")
    Write_names_tex(output, name_array)
    Write_numbers_cols_tex(output,numbers,name_array,res_dict,"ave",comment)
    
    output.write("\nMin\n")
    Write_names_tex(output, name_array)
    Write_numbers_cols_tex(output,numbers,name_array,res_dict,"min",comment)
    
    output.write("\nMax\n")
    Write_names_tex(output, name_array)
    Write_numbers_cols_tex(output,numbers,name_array,res_dict,"max",comment)

    #tabs
    output.write("\n\nTabs format last\n\nAverage\n")
    Write_names_sep(output, name_array, '\t')
    Write_numbers_sep_col(output,numbers,name_array,res_dict,"ave",
                           '\t',comment)
    output.write("Min\n")
    Write_names_sep(output, name_array, '\t')
    Write_numbers_sep_col(output,numbers,name_array,res_dict,"ave",
                           '\t',comment)
    output.write("Max\n")
    Write_names_sep(output, name_array, '\t')
    Write_numbers_sep_col(output,numbers,name_array,res_dict,"ave",
                           '\t',comment)

    #by number, tabs
    output.write("\n\nTabs format detail by number\n\nAverage\n")
    Write_numbers_sep_nr(output,numbers,name_array,res_dict,"ave",
                         '\t', comment, how_many)
    output.write("\nMin\n")
    Write_numbers_sep_nr(output,numbers,name_array,res_dict,"min",
                         '\t', comment, how_many)
    output.write("\nMax\n")
    Write_numbers_sep_nr(output,numbers,name_array,res_dict,"max",
                         '\t', comment, how_many)
    
    #by name, tabs
    output.write("\n\nTabs format detail by method\n\nAverage\n")
    Write_numbers_sep_name(output,numbers,name_array,res_dict,"ave",
                         '\t', comment, how_many)
    output.write("\nMin\n")
    Write_numbers_sep_name(output,numbers,name_array,res_dict,"min",
                         '\t', comment, how_many)
    output.write("\nMax\n")
    Write_numbers_sep_name(output,numbers,name_array,res_dict,"max",
                         '\t', comment, how_many)
    
    output.close()
            
if __name__ == '__main__' :
    #Process_results(name, path_name, res_name, criteria)
    #Process_results("dg", "../Results/cluster", "res_dg50_2.44_eps.txt", "g")
    Process_results("dg", "../Results/may05/cluster/",
                    "../cluster_total.txt", "e")
    Process_results("elps", "../Results/may05/ellips/",
                    "../ellips_total.txt", "e")
    Process_results("sp", "../Results/may05/spiral/",
                    "../spiral_total.txt", "e")
    Process_results("thr", "../Results/may05/torus/",
                    "../torus_total.txt", "e")
    Process_results("thrd", "../Results/may05/torusd/",
                    "../torusd_total.txt", "e")
    Process_results("ukn", "../Results/may05/uk_var/",
                    "../uk_var_total.txt", "e")
    Process_results("ukn", "../Results/may05/uk_uvar/",
                    "../uk_uvar_total.txt", "e")
#    Process_results("dg100", "../Results/may05/cluster/",
#                    "../dg100.txt", "e")
#    Process_results("dg200_49", "../Results/may05/cluster/",
#                    "../dg200.txt", "e")
    #Process_results("dg", "../Results/timing10/results/random",
    #                "../../timing_ran05.txt", "m")
    #Process_results("dg", "../Results/timing10/results/tension",
    #                "../../timing_ten05.txt", "m")
    
    # Old versions of these functions:
    #Process_results("dg", ['bf','r','tv'], "../Results/mc_graph", "dg_res_1000.txt", "dg", 1000, 10)
    #Process_results_limit("ukn", ['bf','r','tv'], "../Results/mc_graph", "res_ukn_200.txt", "ukn", 1000, 10, 200)
