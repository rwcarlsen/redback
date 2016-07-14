/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef RedbackMechMaterial_UO_DC_YSUO_H
#define RedbackMechMaterial_UO_DC_YSUO_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"
#include "RotationTensor.h"
//#include "FiniteStrainPlasticMaterial.h"

#include "RedbackElementParameters.h"

#include "TensorMechanicsPlasticModel.h"

// Forward Declarations
class RedbackMechMaterial_UO_DC_YSUO;

template <>
InputParameters validParams<RedbackMechMaterial_UO_DC_YSUO>();

/**
 * This class copy-pastes the stack from tensorMechanics module
 * TODO: add required features to moose and use proper tensorMechanics module
 */
class RedbackMechMaterial_UO_DC_YSUO : public Material
{
public:
  RedbackMechMaterial_UO_DC_YSUO(const InputParameters & parameters);

  static MooseEnum damageMethodEnum();
  enum DamageMethod
  {
    BrittleDamage,
    CreepDamage,
    BreakageMechanics,
    DamageHealing,
    FromMultiApp
  };

protected:
  // Copy-paste from TensorMechanicsMaterial.h
  virtual void computeProperties();
  virtual void stepInitQpProperties();
  virtual void computeQpElasticityTensor();
  //virtual void computeStrain();
  virtual void computeQpStrain();
  virtual void computeQpStrain(const RankTwoTensor & Fhat); // from FiniteStrainMaterial.h
  virtual void computeQpStress();
  virtual void initQpStatefulProperties(); // from FiniteStrainMaterial.h

  const VariableGradient & _grad_disp_x;
  const VariableGradient & _grad_disp_y;
  const VariableGradient & _grad_disp_z;

  const VariableGradient & _grad_disp_x_old;
  const VariableGradient & _grad_disp_y_old;
  const VariableGradient & _grad_disp_z_old;

  MaterialProperty<RankTwoTensor> & _stress;
  MaterialProperty<RankTwoTensor> & _total_strain;
  MaterialProperty<RankTwoTensor> & _elastic_strain;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor; // now supplied by elasticity tensor material
  MaterialProperty<RankFourTensor> & _Jacobian_mult;

  /// Individual material information
 // RankFourTensor _Cijkl;  // should now be supplied by elasticity tensor

  // MaterialProperty<RankTwoTensor> & _d_stress_dT;
  // RankTwoTensor _strain_increment;

  /// Current deformation gradient
  // RankTwoTensor _dfgrd;

  // const VariableValue & _T;

  // Copy-paste from FiniteStrainMaterial.h
  const MaterialProperty<RankTwoTensor> & _strain_rate;
  const MaterialProperty<RankTwoTensor> & _strain_increment;
  MaterialProperty<RankTwoTensor> & _total_strain_old;
  MaterialProperty<RankTwoTensor> & _elastic_strain_old;
  MaterialProperty<RankTwoTensor> & _stress_old;
  const MaterialProperty<RankTwoTensor> & _rotation_increment;
  MaterialProperty<RankTwoTensor> & _dfgrd;

  // Copy-paste from FiniteStrainPlasticMaterial.h
  std::vector<Real> _yield_stress_vector;
  MaterialProperty<RankTwoTensor> & _plastic_strain;
  MaterialProperty<RankTwoTensor> & _plastic_strain_old;
  MaterialProperty<Real> & _eqv_plastic_strain;
  MaterialProperty<Real> & _eqv_plastic_strain_old;

  // virtual Real yieldFunction(const RankTwoTensor & stress, const Real
  // yield_stress);
  Real getSigEqv(const RankTwoTensor & stress);
  Real deltaFunc(unsigned int i, unsigned int j);
  Real getYieldStress(const Real equivalent_plastic_strain);

  // Copy-paste from FiniteStrainPlasticRateMaterial.h
  virtual void returnMap(const RankTwoTensor &,
                         const RankTwoTensor &,
                         const RankFourTensor &,
                         RankTwoTensor &,
                         RankTwoTensor &,
                         Real &,
                         Real &);



  Real _ref_pe_rate;
  Real _exponent;
  Real _chemo_mechanical_porosity_coeff;

  Real macaulayBracket(Real);

  // Redback specific
//  Real _youngs_modulus;
//  Real _poisson_ratio;
  MaterialProperty<Real> & _mises_stress;
  MaterialProperty<Real> & _mean_stress;
  MaterialProperty<Real> & _mises_strain_rate;
  MaterialProperty<Real> & _volumetric_strain;
  MaterialProperty<Real> & _volumetric_strain_rate;
  MaterialProperty<Real> & _total_volumetric_strain;
  MaterialProperty<Real> & _mechanical_porosity;
  MaterialProperty<Real> & _mass_removal_rate;
  MaterialProperty<Real> & _poromech_kernel;
  MaterialProperty<Real> & _poromech_jac;
  MaterialProperty<Real> & _mod_gruntfest_number;
  MaterialProperty<Real> & _mechanical_dissipation_mech;
  MaterialProperty<Real> & _mechanical_dissipation_jac_mech;
  MaterialProperty<Real> & _damage_kernel;
  MaterialProperty<Real> & _damage_kernel_jac;
  Real _damage_coeff, _dmg_exponent, _healing_coeff;

  Real _exponential;
  // const VariableValue & _dispx_dot;
  // const VariableValue & _dispy_dot;
  // const VariableValue & _dispz_dot;

  // MaterialProperty<RealVectorValue> & _solid_velocity;

  // Using variables
  bool _has_T;
  const VariableValue & _T;
  const VariableValue & _T_old;
  bool _has_pore_pres;
  const VariableValue & _pore_pres;
  const VariableValue & _total_porosity;
  bool _has_D;
  const VariableValue & _damage;
  const VariableValue & _damage_old;

  DamageMethod _damage_method;


  // pointer to the common material parameters
  const RedbackElementParameters* _common_redback_material_parameters;

  // ultimately the following parameters will be extracted from the common parameters above
  // - ultimately this class will be completely overhauled - need to apply changes to redback material first

  const RedbackMaterialParameterUserObject* _gr_uo;
  const RedbackMaterialParameterUserObject* _ar_uo;
  const RedbackMaterialParameterUserObject* _confining_pressure_uo;
  const RedbackMaterialParameterUserObject* _alpha_1_uo;
  const RedbackMaterialParameterUserObject* _alpha_2_uo;
  const RedbackMaterialParameterUserObject* _alpha_3_uo;

  const RedbackMaterialParameterUserObject* _delta_uo;
  const RedbackMaterialParameterUserObject* _solid_thermal_expansion_uo;
  const RedbackMaterialParameterUserObject* _solid_compressibility_uo;
  const RedbackMaterialParameterUserObject* _peclet_number_uo;

  const TensorMechanicsPlasticModel* _plastic_model;


  const MaterialProperty<Real> & _mixture_compressibility;

  const MaterialProperty<Real> & _lewis_number;


  MaterialProperty<Real> & _returnmap_iter;  // needed?

  Real _T0_param, _P0_param;

  Real _damage_dissipation;


  // The following functions are needed in the return map, but the definition
   // is dependent on the yield criterion. Therefore we define them as abstract
   // virtual functions here such that no implementation is needed in
   // RedbackMechMaterial_UO_DC_YSUO.C
   /*
   virtual void
   getJac(const RankTwoTensor &, const RankFourTensor &, Real, Real, Real, Real, Real, Real, RankFourTensor &) = 0;
   virtual void getFlowTensor(const RankTwoTensor &, Real, Real, Real, RankTwoTensor &) = 0;
   virtual Real getFlowIncrement(Real, Real, Real, Real, Real) = 0;
   virtual void get_py_qy(Real, Real, Real &, Real &, Real) = 0;
   */
   // now we will try to derive these from the plastic model
   void getJac(const RankTwoTensor &, const RankFourTensor &, Real, Real, Real, Real, Real, Real, RankFourTensor &);
   void getFlowTensor(const RankTwoTensor &, Real, Real, Real, RankTwoTensor &);


   //void get_py_qy(Real, Real, Real &, Real &, Real);
  //virtual void get_py_qy_damaged(Real, Real, Real &, Real &, Real);

   virtual void get_py_qy_damaged(const RankTwoTensor & trial_stress,
		                          const RankFourTensor & E_ijkl,
						          Real & p_y, Real & q_y, Real equivPlasticStrain);

   virtual void get_py_qy(const RankTwoTensor & trial_stress,
		                 const RankFourTensor & E_ijkl,
						 Real & p_y, Real & q_y, Real equivPlasticStrain);

  ////

  virtual void computeRedbackTerms(RankTwoTensor &, Real, Real);
  virtual void form_damage_kernels(Real);

  virtual void formDamageDissipation(RankTwoTensor &);
  virtual void formBrittleDamage();
  virtual void formCreepDamage(Real);

  // Redback flow increment and derivative
  Real getFlowIncrement(Real sig_eqv, Real p, Real q_y, Real p_y, Real equivPlasticStrain);
  Real getDerivativeFlowIncrement(Real pressure, Real sig_eqv, Real q_yield_stress, Real p_yield_stress);

};

#endif // RedbackMechMaterial_UO_DC_YSUO_H