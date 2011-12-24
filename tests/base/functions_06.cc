//-----------------------------------------------------------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2007, 2008, 2010, 2011 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//-----------------------------------------------------------------------------

// Test VectorFunctionFromScalarFunctionObject

#include "../tests.h"
#include <deal.II/base/function.h>
#include <deal.II/lac/vector.h>


template <int dim>
void check1 ()
{
  VectorFunctionFromScalarFunctionObject<dim>
    object (&Point<dim>::norm, 1, 3);

  Assert (object.n_components == 3, ExcInternalError());

  for (unsigned int i=0; i<10; ++i)
    {
      Point<dim> p;
      for (unsigned int d=0; d<dim; ++d)
	p[d] = i+d;

      for (unsigned int c=0; c<3; ++c)
	Assert (object.value(p,c) == (c==1 ? p.norm() : 0),
		ExcInternalError());

      Vector<double> v(3);
      object.vector_value (p, v);
      for (unsigned int c=0; c<3; ++c)
	Assert (v(c) == (c==1 ? p.norm() : 0),
		ExcInternalError());
    }

  deallog << "OK" << std::endl;
}


template <int dim>
void check2 ()
{
  Point<dim> q;
  for (unsigned int d=0; d<dim; ++d)
    q[d] = d;

  ScalarFunctionFromFunctionObject<dim>
    object (std_cxx1x::bind (&Point<dim>::distance,
			     q,
			     std_cxx1x::_1));

  for (unsigned int i=0; i<10; ++i)
    {
      Point<dim> p;
      for (unsigned int d=0; d<dim; ++d)
	p[d] = i+d;

      Assert (object.value(p) == q.distance (p),
	      ExcInternalError());
    }

  deallog << "OK" << std::endl;
}


int main()
{
  std::string logname = JobIdentifier::base_name(__FILE__) + std::string("/output");
  std::ofstream logfile(logname.c_str());
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  check1<1> ();
  check1<2> ();
  check1<3> ();

  check2<1> ();
  check2<2> ();
  check2<3> ();
}

