/*
    DataViewer visualization package
    Copyright (C) 1997 Randy Paffenroth and Thomas Stone

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/
#include "DVpython_problems.h"
#include <stdlib.h>
#include <cstring>

void DVpython_problems(char *input_problem,
		       DVgeometry *set)
{
  if(strcmp(input_problem,"default_problem")==0) {
    set->set_user_function(default_problem);

  } else if(strcmp(input_problem,"mc_graph")==0) {
    set->set_user_function(mc_graph);

  } else if(strcmp(input_problem,"graph_ga")==0) {
    set->set_user_function(graph_ga);

  } else if(strcmp(input_problem,"mc_combine")==0) {
    set->set_user_function(mc_combine);

  } else if(strcmp(input_problem,"mc_view")==0) {
    set->set_user_function(mc_view);

  } else if(strcmp(input_problem,"view_res")==0) {
    set->set_user_function(view_res);

  } else {
    set->set_user_function(default_problem);
    cerr << "Default problem being used" << endl;
  }
}

