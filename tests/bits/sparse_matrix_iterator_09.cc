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



// this test, extracted from dof_constraints_09 and
// sparse_matrix_iterator_09, used to fail with aborts

#include "../tests.h"
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/sparse_matrix.h>
#include <fstream>
#include <iomanip>


void test ()
{
  // create a sparsity pattern with totally
  // empty lines (not even diagonals, since
  // not quadratic)
  SparsityPattern sparsity(4,5,1);
  sparsity.add (1,1);
  sparsity.add (3,1);
  sparsity.compress ();

  // attach a sparse matrix to it
  SparseMatrix<double> A(sparsity);

  // and loop over the elements of it
  for (SparseMatrix<double>::const_iterator k=A.begin();
       k!=A.end(); ++k)
    deallog << k->row() << ' ' << k->column() << ' ' << k->value()
            << std::endl;
}



int main ()
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  try
    {
      test ();
    }
  catch (std::exception &exc)
    {
      deallog << std::endl << std::endl
              << "----------------------------------------------------"
              << std::endl;
      deallog << "Exception on processing: " << std::endl
              << exc.what() << std::endl
              << "Aborting!" << std::endl
              << "----------------------------------------------------"
              << std::endl;

      return 1;
    }
  catch (...)
    {
      deallog << std::endl << std::endl
              << "----------------------------------------------------"
              << std::endl;
      deallog << "Unknown exception!" << std::endl
              << "Aborting!" << std::endl
              << "----------------------------------------------------"
              << std::endl;
      return 1;
    };
}
