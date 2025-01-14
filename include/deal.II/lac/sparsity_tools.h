// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2008 - 2014 by the deal.II authors
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

#ifndef __deal2__sparsity_tools_h
#define __deal2__sparsity_tools_h


#include <deal.II/base/config.h>
#include <deal.II/base/exceptions.h>
#include <deal.II/lac/sparsity_pattern.h>

#include <vector>

#ifdef DEAL_II_WITH_MPI
#include <mpi.h>
#include <deal.II/base/index_set.h>
#endif

DEAL_II_NAMESPACE_OPEN


/*! @addtogroup Sparsity
 *@{
 */

/**
 * A namespace for functions that deal with things that one can do on sparsity
 * patterns, such as renumbering rows and columns (or degrees of freedom if
 * you want) according to the connectivity, or partitioning degrees of
 * freedom.
*/
namespace SparsityTools
{
  /**
   * Use the METIS partitioner to generate a partitioning of the degrees of
   * freedom represented by this sparsity pattern. In effect, we view this
   * sparsity pattern as a graph of connections between various degrees of
   * freedom, where each nonzero entry in the sparsity pattern corresponds to
   * an edge between two nodes in the connection graph. The goal is then to
   * decompose this graph into groups of nodes so that a minimal number of
   * edges are cut by the boundaries between node groups. This partitioning is
   * done by METIS. Note that METIS can only partition symmetric sparsity
   * patterns, and that of course the sparsity pattern has to be square. We do
   * not check for symmetry of the sparsity pattern, since this is an
   * expensive operation, but rather leave this as the responsibility of
   * caller of this function.
   *
   * After calling this function, the output array will have values between
   * zero and @p n_partitions-1 for each node (i.e. row or column of the
   * matrix).
   *
   * This function will generate an error if METIS is not installed unless @p
   * n_partitions is one. I.e., you can write a program so that it runs in the
   * single-processor single-partition case without METIS installed, and only
   * requires METIS when multiple partitions are required.
   *
   * Note that the sparsity pattern itself is not changed by calling this
   * function. However, you will likely use the information generated by
   * calling this function to renumber degrees of freedom, after which you
   * will of course have to regenerate the sparsity pattern.
   *
   * This function will rarely be called separately, since in finite element
   * methods you will want to partition the mesh, not the matrix. This can be
   * done by calling @p GridTools::partition_triangulation.
   */
  void partition (const SparsityPattern     &sparsity_pattern,
                  const unsigned int         n_partitions,
                  std::vector<unsigned int> &partition_indices);

  /**
   * For a given sparsity pattern, compute a re-enumeration of row/column
   * indices based on the algorithm by Cuthill-McKee.
   *
   * This algorithm is a graph renumbering algorithm in which we attempt to
   * find a new numbering of all nodes of a graph based on their connectivity
   * to other nodes (i.e. the edges that connect nodes). This connectivity is
   * here represented by the sparsity pattern. In many cases within the
   * library, the nodes represent degrees of freedom and edges are nonzero
   * entries in a matrix, i.e. pairs of degrees of freedom that couple through
   * the action of a bilinear form.
   *
   * The algorithms starts at a node, searches the other nodes for those which
   * are coupled with the one we started with and numbers these in a certain
   * way. It then finds the second level of nodes, namely those that couple
   * with those of the previous level (which were those that coupled with the
   * initial node) and numbers these. And so on. For the details of the
   * algorithm, especially the numbering within each level, we refer the
   * reader to the book of Schwarz (H. R. Schwarz: Methode der finiten
   * Elemente).
   *
   * These algorithms have one major drawback: they require a good starting
   * node, i.e. node that will have number zero in the output array. A
   * starting node forming the initial level of nodes can thus be given by the
   * user, e.g. by exploiting knowledge of the actual topology of the
   * domain. It is also possible to give several starting indices, which may
   * be used to simulate a simple upstream numbering (by giving the inflow
   * nodes as starting values) or to make preconditioning faster (by letting
   * the Dirichlet boundary indices be starting points).
   *
   * If no starting index is given, one is chosen automatically, namely one
   * with the smallest coordination number (the coordination number is the
   * number of other nodes this node couples with). This node is usually
   * located on the boundary of the domain. There is, however, large ambiguity
   * in this when using the hierarchical meshes used in this library, since in
   * most cases the computational domain is not approximated by tilting and
   * deforming elements and by plugging together variable numbers of elements
   * at vertices, but rather by hierarchical refinement. There is therefore a
   * large number of nodes with equal coordination numbers. The renumbering
   * algorithms will therefore not give optimal results.
   *
   * If the graph has two or more unconnected components and if no starting
   * indices are given, the algorithm will number each component
   * consecutively. However, this requires the determination of a starting
   * index for each component; as a consequence, the algorithm will produce an
   * exception if starting indices are given, taking the latter as an
   * indication that the caller of the function would like to override the
   * part of the algorithm that chooses starting indices.
   */
  void
  reorder_Cuthill_McKee (const SparsityPattern     &sparsity,
                         std::vector<SparsityPattern::size_type> &new_indices,
                         const std::vector<SparsityPattern::size_type> &starting_indices = std::vector<SparsityPattern::size_type>());


#ifdef DEAL_II_WITH_MPI
  /**
   * Communciate rows in a compressed sparsity pattern over MPI.
   *
   * @param csp is the sparsity pattern that has been built locally and for
   * which we need to exchange entries with other processors to make sure that
   * each processor knows all the elements of the rows of a matrix it stores
   * and that may eventually be written to. This sparsity pattern will be
   * changed as a result of this function: All entries in rows that belong to
   * a different processor are sent to them and added there.
   *
   * @param rows_per_cpu determines ownership of rows.
   *
   * @param mpi_comm is the MPI communicator that is shared between the
   * processors that all participate in this operation.
   *
   * @param myrange indicates the range of elements stored locally and should
   * be the one used in the constructor of the
   * CompressedSimpleSparsityPattern.  This should be the locally relevant
   * set.  Only rows contained in myrange are checked in csp for transfer.
   * This function needs to be used with PETScWrappers::MPI::SparseMatrix for
   * it to work correctly in a parallel computation.
   */
  template <class CSP_t>
  void distribute_sparsity_pattern(CSP_t &csp,
                                   const std::vector<typename CSP_t::size_type> &rows_per_cpu,
                                   const MPI_Comm &mpi_comm,
                                   const IndexSet &myrange);

  /**
   * similar to the function above, but includes support for
   * BlockCompressedSimpleSparsityPattern.
   * @p owned_set_per_cpu is typically DoFHandler::locally_owned_dofs_per_processor
   * and @p myrange are locally_relevant_dofs.
   */
  template <class CSP_t>
  void distribute_sparsity_pattern(CSP_t &csp,
                                   const std::vector<IndexSet> &owned_set_per_cpu,
                                   const MPI_Comm &mpi_comm,
                                   const IndexSet &myrange);

#endif


  /**
   * Exception
   */
  DeclException0 (ExcMETISNotInstalled);
  /**
   * Exception
   */
  DeclException1 (ExcInvalidNumberOfPartitions,
                  int,
                  << "The number of partitions you gave is " << arg1
                  << ", but must be greater than zero.");

  /**
   * Exception
   */
  DeclException1 (ExcMETISError,
                  int,
                  << "    An error with error number " << arg1
                  << " occurred while calling a METIS function");

  /**
   * Exception
   */
  DeclException2 (ExcInvalidArraySize,
                  int, int,
                  << "The array has size " << arg1 << " but should have size "
                  << arg2);
}

/**
 *@}
 */

DEAL_II_NAMESPACE_CLOSE

#endif
