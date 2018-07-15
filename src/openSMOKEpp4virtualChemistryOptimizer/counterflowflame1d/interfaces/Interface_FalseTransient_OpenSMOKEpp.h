/*----------------------------------------------------------------------*\
|    ___                   ____  __  __  ___  _  _______                  |
|   / _ \ _ __   ___ _ __ / ___||  \/  |/ _ \| |/ / ____| _     _         |
|  | | | | '_ \ / _ \ '_ \\___ \| |\/| | | | | ' /|  _| _| |_ _| |_       |
|  | |_| | |_) |  __/ | | |___) | |  | | |_| | . \| |__|_   _|_   _|      |
|   \___/| .__/ \___|_| |_|____/|_|  |_|\___/|_|\_\_____||_|   |_|        |
|        |_|                                                              |
|                                                                         |
|   Author: Alberto Cuoci <alberto.cuoci@polimi.it>                       |
|   CRECK Modeling Group <http://creckmodeling.chem.polimi.it>            |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
|-------------------------------------------------------------------------|
|                                                                         |
|   This file is part of OpenSMOKE++ Suite.                               |
|                                                                         |
|   Copyright(C) 2015, 2014, 2013  Alberto Cuoci                          |
|   Source-code or binary products cannot be resold or distributed        |
|   Non-commercial use only                                               |
|   Cannot modify source-code for any purpose (cannot create              |
|   derivative works)                                                     |
|                                                                         |
\*-----------------------------------------------------------------------*/

#include "math/native-nls-solvers/NonLinearSystemSolver"

class OpenSMOKE_Flame1D_MyFalseTransientSystem_OpenSMOKEpp_CounterFlowFlame1D
{
public:

	OpenSMOKE_Flame1D_MyFalseTransientSystem_OpenSMOKEpp_CounterFlowFlame1D() {};

	double deltat() const { return deltat_; }

private:

	OpenSMOKE::OpenSMOKE_CounterFlowFlame1D *ptFlame;
	double deltat_;
	Eigen::VectorXd yInitial_;
	std::vector<OpenSMOKE::EquationType> indices_differential_algebraic_;


protected:

	unsigned int ne_;

	void MemoryAllocation()
	{
	}

	virtual void Equations(const Eigen::VectorXd &y, Eigen::VectorXd &f)
	{
		ptFlame->Equations(0., y.data(), f.data());

		for (int i = 0; i < y.size(); i++)
			if (indices_differential_algebraic_[i] == OpenSMOKE::EQUATION_TYPE_DIFFERENTIAL)
				f(i) = y(i) - yInitial_(i) - f(i) * deltat_;
	}

	void Jacobian(const Eigen::VectorXd &y, Eigen::MatrixXd &J)
	{
	};

	void Print(const int call, const double t, const double phiW, const Eigen::VectorXd &x, const Eigen::VectorXd &f)
	{
		flame_cfdf->Print(x.data(), phiW);
	}

public:

	void SetInitialConditions(const Eigen::VectorXd& yInitial)
	{
		yInitial_ = yInitial;
	}

	void SetDifferentialAlgebraic(const std::vector<OpenSMOKE::EquationType>& indices_differential_algebraic)
	{
		indices_differential_algebraic_ = indices_differential_algebraic;
	}

	void SetTimeStep(const double deltat)
	{
		deltat_ = deltat;
	}

	void assign(OpenSMOKE::OpenSMOKE_CounterFlowFlame1D *flame)
	{
		ptFlame = flame;
	}
};

#include "math/native-nls-solvers/interfaces-false-transient/Band_OpenSMOKEppFalseTransient.h"
#include "math/native-nls-solvers/interfaces-false-transient/Sparse_OpenSMOKEppFalseTransient.h"
