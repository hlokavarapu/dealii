// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2007 - 2013 by the deal.II authors
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



// check FullMatrix::determinant


#include "../tests.h"
#include "full_matrix_common.h"


std::string output_file_name = "output";


template <typename number>
void
check ()
{
  for (unsigned int n=1; n<=3; ++n)
    {
      const number array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      FullMatrix<number> m(n,n, array);
      print_matrix (m);
      deallog << m.determinant () << std::endl;
    }
}


