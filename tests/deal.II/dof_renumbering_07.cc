//----------------------------------------------------------------------
//    $Id: dof_renumbering_04.cc 27160 2012-10-22 14:26:35Z kanschat $
//    Version: $Name$
//
//    Copyright (C) 2000, 2001, 2003, 2004, 2007, 2008, 2010, 2012 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------------------------------------------------

// Check DoFRenumbering::boost::Cuthill_McKee


#include "../tests.h"
#include <deal.II/base/logstream.h>
#include <deal.II/base/function_lib.h>
#include <deal.II/lac/vector.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_renumbering.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_dgq.h>
#include <deal.II/fe/fe_dgp.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/fe_values.h>

#include <fstream>



template <int dim>
void
print_dofs (const DoFHandler<dim> &dof)
{
  const FiniteElement<dim>& fe = dof.get_fe();
  std::vector<unsigned int> v (fe.dofs_per_cell);
  std_cxx1x::shared_ptr<FEValues<dim> > fevalues;

  if (fe.has_support_points())
    {
      Quadrature<dim> quad(fe.get_unit_support_points());
      fevalues = std_cxx1x::shared_ptr<FEValues<dim> >(new FEValues<dim>(fe, quad, update_q_points));
    }

  for (typename DoFHandler<dim>::active_cell_iterator cell=dof.begin_active();
       cell != dof.end(); ++cell)
    {
      Point<dim> p = cell->center();
      if (fevalues.get() != 0)
	fevalues->reinit(cell);

      cell->get_dof_indices (v);
      for (unsigned int i=0; i<v.size(); ++i)
	if (fevalues.get() != 0)
	  deallog << fevalues->quadrature_point(i) << '\t' << v[i] << std::endl;
	else
	  deallog << p << '\t' << v[i] << std::endl;
      deallog << std::endl;
    }
}



template <int dim>
void
check_renumbering(DoFHandler<dim>& dof)
{
  const FiniteElement<dim>& element = dof.get_fe();
  deallog << element.get_name() << std::endl;

  DoFRenumbering::boost::Cuthill_McKee(dof);
  for (unsigned int level=0;level<dof.get_tria().n_levels();++level)
    DoFRenumbering::Cuthill_McKee(dof, level);
  print_dofs (dof);
}


template <int dim>
void
check ()
{
  Triangulation<dim> tr;
  GridGenerator::hyper_cube(tr, -1., 1.);
  tr.refine_global (1);
  tr.begin_active()->set_refine_flag ();
  tr.execute_coarsening_and_refinement ();
  tr.refine_global(3-dim);

  {
    FE_Q<dim> fe(2);
    DoFHandler<dim> dof(tr);

    dof.distribute_dofs(fe);
    dof.distribute_mg_dofs(fe);
    check_renumbering(dof);
  }

  {
    FESystem<dim> fe(FE_DGQ<dim>(1),1,
		     FE_Q<dim>(2),dim);
    DoFHandler<dim> dof(tr);

    dof.distribute_dofs(fe);
    dof.distribute_mg_dofs(fe);
    check_renumbering(dof);
  }
}


int main ()
{
  std::ofstream logfile ("dof_renumbering_07/output");
  deallog << std::setprecision (2);
  deallog << std::fixed;
  deallog.attach(logfile);
  deallog.depth_console (0);

  deallog.push ("1d");
  check<1> ();
  deallog.pop ();
  deallog.push ("2d");
  check<2> ();
  deallog.pop ();
  deallog.push ("3d");
  check<3> ();
  deallog.pop ();
}