// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2003 - 2013 by the deal.II authors
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



// check the cells generated by the CylinderBoundary the default
// axiparallel cylinder

#include "../tests.h"
#include <deal.II/base/logstream.h>
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_boundary_lib.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/mapping_c1.h>

#include <fstream>



template <int dim>
void check ()
{
  Triangulation<dim> triangulation;
  GridGenerator::cylinder (triangulation);
  static const CylinderBoundary<dim> boundary (1,0);
  triangulation.set_boundary (0, boundary);
  triangulation.refine_global (2);

  for (typename Triangulation<dim>::active_cell_iterator
       cell = triangulation.begin_active();
       cell!=triangulation.end(); ++cell)
    for (unsigned int i=0; i<GeometryInfo<dim>::vertices_per_cell; ++i)
      deallog << cell->vertex(i) << std::endl;
}


int main ()
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  check<2> ();
  check<3> ();
}



