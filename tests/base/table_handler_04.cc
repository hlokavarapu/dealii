// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2010 - 2013 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// pad columns if we don't fill them in each iteration


#include "../tests.h"
#include <deal.II/base/data_out_base.h>
#include <deal.II/base/table_handler.h>
#include <deal.II/base/logstream.h>

#include <vector>
#include <iomanip>
#include <fstream>
#include <string>


int main ()
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  TableHandler table;
  table.set_auto_fill_mode (true);

  std::string keys[3] = { "key1", "key2", "key3" };

  table.add_value(keys[0], 0);
  table.add_value(keys[0], 1);
  table.add_value(keys[0], 2);
  table.add_value(keys[1], 13);
  table.add_value(keys[2], std::string("a"));

  table.write_text(deallog.get_file_stream());
}
