// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2005 - 2013 by the deal.II authors
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



// test VectorTools::interpolate_boundary_values for codim=1. like _01
// but for 1d triangulations
//
// the function ran into an endless loop whenever the 1d codim=1 mesh
// was topologically a circle, since it tries to find the left or
// right most vertex by starting at an arbitrary cell and always going
// to the left/right. If the mesh is closed, this of course doesn't
// work so well.

#include "../tests.h"
#include <fstream>
#include <deal.II/base/logstream.h>
#include <deal.II/base/function_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/numerics/vector_tools.h>

#include <string>

using namespace dealii;


std::ofstream logfile("output");

void test()
{
  const unsigned int dim = 1;
  const unsigned int spacedim=2;

  Triangulation<dim,spacedim> tria;
  std::map<Triangulation<dim,spacedim>::cell_iterator,Triangulation<spacedim,spacedim>::face_iterator >
  surface_to_volume_mapping;
  Triangulation<spacedim> volume_mesh;
  GridGenerator::hyper_cube(volume_mesh);

  surface_to_volume_mapping
    = GridGenerator::extract_boundary_mesh (volume_mesh, tria);

  FE_Q<dim,spacedim> fe(2);
  DoFHandler<dim,spacedim> dof_handler (tria);
  dof_handler.distribute_dofs (fe);

  deallog << dof_handler.n_dofs() << " degrees of freedom" << std::endl;

  // test left and right boundary
  // separatel
  for (unsigned int boundary_id=0; boundary_id<2; ++boundary_id)
    {
      std::map<types::global_dof_index, double> bv;
      VectorTools::interpolate_boundary_values (dof_handler,
                                                boundary_id,
                                                Functions::SquareFunction<spacedim>(),
                                                bv);
      deallog << bv.size() << " boundary degrees of freedom" << std::endl;

      for (std::map<types::global_dof_index, double>::const_iterator i = bv.begin();
           i != bv.end(); ++i)
        deallog << i->first << ' ' << i->second << std::endl;

      for (DoFHandler<dim,spacedim>::active_cell_iterator
           cell = dof_handler.begin_active(); cell != dof_handler.end(); ++cell)
        for (unsigned int f=0; f<GeometryInfo<dim>::faces_per_cell; ++f)
          if (cell->at_boundary(f) &&
              (cell->face(f)->boundary_indicator() == boundary_id))
            for (unsigned int v=0; v<GeometryInfo<dim>::vertices_per_face; ++v)
              for (unsigned int i=0; i<fe.dofs_per_vertex; ++i)
                {
                  Assert (bv.find(cell->face(f)->vertex_dof_index(v,i))
                          != bv.end(),
                          ExcInternalError());
                  Assert (bv[cell->face(f)->vertex_dof_index(v,i)]
                          ==
                          Functions::SquareFunction<spacedim>()
                          .value(cell->face(f)->vertex(v),i),
                          ExcInternalError());
                }
    }
}



int main ()
{
  deallog.attach(logfile);
  deallog.depth_console(0);

  test();

  return 0;
}

