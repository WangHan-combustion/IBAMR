// Filename: IBMovingTargetPointForceGen.h
// Created on 14 Aug 2008 by Boyce Griffith
//
// Copyright (c) 2002-2010, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of New York University nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef included_IBMovingTargetPointForceGen
#define included_IBMovingTargetPointForceGen

/////////////////////////////// INCLUDES /////////////////////////////////////

// IBAMR INCLUDES
#include <ibamr/IBLagrangianForceStrategy.h>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{

/*!
 * \brief Class IBMovingTargetPointForceGen is a concrete
 * IBLagrangianForceStrategy that computes the penalty forces generated by a
 * collection of moving target points.
 *
 * \note Class IBMovingTargetPointForceGen DOES NOT correct for periodic
 * displacements of IB points.
 */
class IBMovingTargetPointForceGen
    : public IBLagrangianForceStrategy
{
public:
    /*!
     * \brief Default constructor.
     */
    IBMovingTargetPointForceGen(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db=NULL);

    /*!
     * \brief Virtual destructor.
     */
    virtual
    ~IBMovingTargetPointForceGen();

    /*!
     * \brief Register a target point position and velocity specification
     * function with the force generator.
     */
    void
    registerPositionAndVelocityFunction(
        const int spec_fcn_index,
        void (*spec_fcn)(double X_target[NDIM], double U_target[NDIM], const double X[NDIM], const double U[NDIM], const double& data_time, const int& lag_mastr_idx));

    /*!
     * \brief Compute the penalty forces determined with the present
     * configuration of the Lagrangian mesh.
     *
     * \note Nodal forces computed by this method are \em added to the force
     * vector.
     */
    virtual void
    computeLagrangianForce(
        SAMRAI::tbox::Pointer<IBTK::LMeshData> F_data,
        SAMRAI::tbox::Pointer<IBTK::LMeshData> X_data,
        SAMRAI::tbox::Pointer<IBTK::LMeshData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the non-zero structure of the force Jacobian matrix.
     *
     * \note Elements indices must be global PETSc indices.
     */
    virtual void
    computeLagrangianForceJacobianNonzeroStructure(
        std::vector<int>& d_nnz,
        std::vector<int>& o_nnz,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the Jacobian of the force with respect to the present
     * structure configuration and velocity.
     *
     * \note The elements of the Jacobian should be "accumulated" in the
     * provided matrix J.
     *
     * \note A default implementation is provided with results in an assertion
     * failure for structures for which no Jacobian is available.
     */
    virtual void
    computeLagrangianForceJacobian(
        Mat& J_mat,
        MatAssemblyType assembly_type,
        const double X_coef,
        SAMRAI::tbox::Pointer<IBTK::LMeshData> X_data,
        const double U_coef,
        SAMRAI::tbox::Pointer<IBTK::LMeshData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the potential energy with respect to the present structure
     * configuration and velocity.
     *
     * \note This method is not actually implemented --- it just prints a
     * warning message and returns 0.0.
     */
    virtual double
    computeLagrangianEnergy(
        SAMRAI::tbox::Pointer<IBTK::LMeshData> X_data,
        SAMRAI::tbox::Pointer<IBTK::LMeshData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBMovingTargetPointForceGen(
        const IBMovingTargetPointForceGen& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBMovingTargetPointForceGen&
    operator=(
        const IBMovingTargetPointForceGen& that);

    /*!
     * \brief Read input values, indicated above, from given database.
     *
     * The database pointer may be null.
     */
    void
    getFromInput(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> db);

    /*!
     * \brief Target point position and velocity functions.
     */
    std::map<int,void (*)(double X_target[NDIM], double U_target[NDIM], const double X[NDIM], const double U[NDIM], const double& data_time, const int& lag_mastr_idx)> d_spec_fcn_map;
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibamr/IBMovingTargetPointForceGen.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBMovingTargetPointForceGen
