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



// like 49, but do the test for
//  TrilinosWrappers::MPI::BlockVector
//         ::operator = (dealii::BlockVector<TrilinosScalar>)
// with block vectors instead of plain vectors

#include "../tests.h"
#include <deal.II/base/utilities.h>
#include <deal.II/lac/trilinos_block_vector.h>
#include <deal.II/lac/block_vector.h>
#include <fstream>
#include <iostream>
#include <vector>


void test (TrilinosWrappers::MPI::BlockVector &v)
{
  std::vector<types::global_dof_index> sizes (2, 3);
  dealii::BlockVector<TrilinosScalar> w (sizes);

  for (unsigned int i=0; i<w.size(); ++i)
    w(i) = i;

  v = w;


  // make sure we get the expected result
  for (unsigned int i=0; i<v.size(); ++i)
    {
      Assert (w(i) == i, ExcInternalError());
      Assert (v(i) == i, ExcInternalError());
    }

  // now also check the reverse assignment
  w = 0;
  w = v;
  for (unsigned int i=0; i<v.size(); ++i)
    {
      Assert (w(i) == i, ExcInternalError());
      Assert (v(i) == i, ExcInternalError());
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
        std::vector<Epetra_Map> sizes;
        Epetra_Map map(TrilinosWrappers::types::int_type(3),
                       TrilinosWrappers::types::int_type(3),
                       0,
                       Utilities::Trilinos::comm_world());
        sizes.push_back (map);
        sizes.push_back (map);

        TrilinosWrappers::MPI::BlockVector v (sizes);
        test (v);
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
