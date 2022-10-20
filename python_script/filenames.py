#!/usr/bin/python/

import string
import os.path

# Adds the parameter x in the name just before the extension.
# It works well for integers, floats, or strings.
# For anything else it just adds an underline.
def append_name(filename, x):
    path=os.path.dirname(filename)
    name=os.path.basename(filename)
    units=string.split(name, ".")
    addstr="_"
    if type(x) == type(1):
        addstr = "%s%d" %(addstr, x)
    elif type(x) == type(0.1):
        if abs(x) >= 1 or x == 0.0:
            # For real numbers bigger than 1, insert just the units
            addstr = "%s%d" %(addstr, abs(int(x)))
        else:
            # for real numbers smaller than 1, insert just the decimals
            str="%g" %(abs(x))
            dec=string.split(str, ".")[1]
            addstr = "%s%s" %(addstr, dec)
    elif type(x) == type("..."):
        addstr = "%s%s" %(addstr, x)
    if (len(units)> 1):
        units[len(units)-2] = string.join([units[len(units)-2], addstr], "")
        new_name = string.join(units, ".")
    else:
        units.append(addstr)
        new_name=units[0]
    return os.path.join(path, new_name)
        
def get_extension(filename):
    parts = string.split(filename,".")
    count = len(parts)
    if len <= 1:
        return ""
    else:
        return parts[count-1]
