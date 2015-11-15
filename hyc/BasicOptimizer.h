#ifndef __BASICOPTMIZER_H
#define __BASICOPTMIZER_H

/*
 *	BasicOptimizer.h
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
 *		
 * 	History:
 *	 	Author			Date			Modify Reason		
 *		----------------------------------------------------------------
 *		Chi-Yi Tsai		2012/09/05		File Creation  
 *		
 *
 */

enum LineSearch 
{
	GoldenSection = 1, 
	Fibonacci
};
enum SolverMethod 
{
	SteepestDecent = 1,
	ConjugateGradient, 
	QuasiNewton
};
enum DifferentialMethod
{
	Forward = 1,
	Backward, 
	Central
};
enum ConjugateFormula 
{
	PolakRibiere = 1, 
	HestenesStiefel,
	FletcherReeves,
	Powell
};
enum QuasiNewtonFormula 
{
	DFP = 1, 
	BFGS
};

typedef double (*pfnScaleCostFun)(double *pfX);

double pfun1(double *pp1);

class CBasicOptimizer
{
public:
	CBasicOptimizer();
	~CBasicOptimizer();

	// return norm value of the gradient vector of the local minimum point
	double	OptScaleCostFunSolver(pfnScaleCostFun pfun, double *pfX, int nDim);

	// return the local minimum point
	double	OptOneDimensionFunSolver(pfnScaleCostFun pfun, double fX);

	void	SetLineSearchMethod(LineSearch Line);
	void	SetSolverMethod(SolverMethod OptMethod);
	void	SetDifferentialMethod(DifferentialMethod Differential);
	void	SetConjugateFormula(ConjugateFormula Formula);
	void	SetQuasiNewtonFormula(QuasiNewtonFormula Formula);

	enum LineSearch			GetLineSearchMethod(void);
	enum SolverMethod		GetSolverMethod(void);
	enum DifferentialMethod	GetDifferentialMethod(void);
	enum ConjugateFormula	GetConjugateFormula(void);
	enum QuasiNewtonFormula	GetQuasiNewtonFormula(void);

protected:


private:	


};

#endif
