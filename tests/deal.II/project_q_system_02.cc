// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2006 - 2013 by the deal.II authors
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



// check that VectorTools::project works for FESystem(FE_Q) elements correctly
// on a uniformly refined mesh for functions of degree q

char logname[] = "output";


#include "project_common.h"


template <int dim>
void test ()
{
  for (unsigned int p=1; p<5-dim; ++p)
    test_with_hanging_nodes (FESystem<dim>(FE_Q<dim>(p), 1,
                                           FE_DGQ<dim>(p+1), 1),
                             p);
}
