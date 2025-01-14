// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2004 - 2013 by the deal.II authors
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



// like _01, except that we use operator[] instead of operator*

#include "../tests.h"
#include <deal.II/base/utilities.h>
#include <deal.II/lac/trilinos_block_vector.h>
#include <fstream>
#include <iostream>


void test ()
{
  TrilinosWrappers::BlockVector v(2);
  for (unsigned int i=0; i<v.n_blocks(); ++i)
    v.block(i).reinit(1);
  v.collect_sizes();

  v(0) = 1;
  v(1) = 2;

  // first check reading through a const
  // iterator
  {
    TrilinosWrappers::BlockVector::const_iterator i=v.begin();
    Assert (i[0] == 1, ExcInternalError());
    Assert (i[1] == 2, ExcInternalError());
  }

  // same, but create iterator in a different
  // way
  {
    TrilinosWrappers::BlockVector::const_iterator
    i=const_cast<const TrilinosWrappers::BlockVector &>(v).begin();
    Assert (i[0] == 1, ExcInternalError());
    Assert (i[1] == 2, ExcInternalError());
  }

  // read through a read-write iterator
  {
    TrilinosWrappers::BlockVector::iterator i = v.begin();
    Assert (i[0] == 1, ExcInternalError());
    Assert (i[1] == 2, ExcInternalError());
  }

  // write through a read-write iterator
  {
    TrilinosWrappers::BlockVector::iterator i = v.begin();
    i[0] = 2;
    i[1] = 3;
  }

  // and read again
  {
    TrilinosWrappers::BlockVector::iterator i = v.begin();
    Assert (i[0] == 2, ExcInternalError());
    Assert (i[1] == 3, ExcInternalError());
  }

  deallog << "OK" << std::endl;
}



int main (int argc,char **argv)
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  Utilities::MPI::MPI_InitFinalize mpi_initialization (argc, argv);


  try
    {
      {
        test ();
      }
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
