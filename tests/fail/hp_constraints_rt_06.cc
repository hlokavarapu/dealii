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



// check that computation of hp constraints works for RT elements correctly

char logname[] = "output";


#include "../hp/hp_constraints_common.h"
#include <deal.II/fe/fe_raviart_thomas.h>


template <int dim>
void test ()
{
  if (dim == 1)
    return;

  hp::FECollection<dim> fe;
  std::vector<unsigned int> degrees;
  for (unsigned int i=1; i<7-dim; ++i)
    {
      fe.push_back (FE_RaviartThomas<dim>(i));
      degrees.push_back (i);
    }

  test_interpolation  (fe, degrees);
}
