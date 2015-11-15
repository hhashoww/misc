/*
 *	BasicOptimizer.cpp
 *
 *	Description:
 *		Basic optimization class includes
 *			1. Basic line search algorithm 
 *			2. Gradient computing
 *			3. Gradient algorithm
 *			4. Conjugate Gradient algorithm 
 *			5. QuasiNewton algorithm
 *
 *		
 * 	History:
 *	 	Author			Date			Modify Reason		
 *		----------------------------------------------------------------
 *		Chi-Yi Tsai		2012/09/05		File Creation    
 *		
 *
 */

//#include "stdafx.h"
#include "BasicOptimizer.h"
#include <math.h>
#include <stdio.h>

enum LineSearch			m_LineSearch;
enum SolverMethod		m_SolverMethod;
enum DifferentialMethod	m_DifferentialMethod;
enum ConjugateFormula	m_ConjugateFormula;
enum QuasiNewtonFormula	m_QuasiNewtonFormula;

double pfun1(double *pp1)
{
	double x = *pp1;
	//return x*2 - 3;
	return pow(x-1.777, 2) + 1;
}

double CBasicOptimizer::OptScaleCostFunSolver(pfnScaleCostFun pfun, double *pfX, int nDim)
{
//	return pfun(pfX);
	return 0;
}

double CBasicOptimizer::OptOneDimensionFunSolver(pfnScaleCostFun pfun, double fX)
{
	return pfun(&fX);
}

#pragma region SET
void CBasicOptimizer::SetLineSearchMethod(LineSearch Line)
{ 
	m_LineSearch = Line;
	printf(".SetLineSearchMethod = %d\n\n", Line);
}
void CBasicOptimizer::SetSolverMethod(SolverMethod OptMethod)
{ 
	m_SolverMethod = OptMethod; 
}
void CBasicOptimizer::SetDifferentialMethod(DifferentialMethod Differential)
{ 
	m_DifferentialMethod = Differential; 
}
void CBasicOptimizer::SetConjugateFormula(ConjugateFormula Formula)
{ 
	m_ConjugateFormula = Formula; 
}
void CBasicOptimizer::SetQuasiNewtonFormula(QuasiNewtonFormula Formula)
{ 
	m_QuasiNewtonFormula = Formula; 
}
#pragma endregion
#pragma region GET
LineSearch CBasicOptimizer::GetLineSearchMethod(void)
{ 
	return m_LineSearch;
}
enum SolverMethod CBasicOptimizer::GetSolverMethod(void)
{ 
	return m_SolverMethod; 
}
DifferentialMethod CBasicOptimizer::GetDifferentialMethod(void)
{ 
	return m_DifferentialMethod; 
}
ConjugateFormula CBasicOptimizer::GetConjugateFormula(void)
{ 
	return m_ConjugateFormula; 
}
QuasiNewtonFormula CBasicOptimizer::GetQuasiNewtonFormula(void)
{ 
	return m_QuasiNewtonFormula; 
}
#pragma endregion
CBasicOptimizer::CBasicOptimizer()
{
	printf("...Create CBasicOptimizer...\n\n");
}

CBasicOptimizer::~CBasicOptimizer()
{
	printf("...Destroy CBasicOptimizer...\n\n");
}
