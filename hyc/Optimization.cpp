// Optimization.cpp : 定義主控台應用程式的進入點。
//

//#include "stdafx.h"
#include "BasicOptimizer.h"
#include <math.h>
#include <stdio.h>

#define RANGE_MIN 0
#define RANGE_MAX 4

#pragma region ClassA
//=ClassA============
// 	class ClassA
// 	{
// 	public:
// 		ClassA();
// 		~ClassA();
// 
// 	protected:
// 	private:	
// 	}; 
// 
// 	ClassA::ClassA() 
// 	{
// 		printf("Create ClassA\n");
// 	}
// 
// 	ClassA::~ClassA() 
// 	{
// 		printf("Destroy ClassA\n");
// 	}
//===================
#pragma endregion

double F(pfnScaleCostFun pfun, double range_min, double range_max)
{
	double d = 0.61803 * (range_max - range_min);
	double x1 = range_max - d;
	double x2 = range_min + d;
	printf("R[ %2.4f, %2.4f ]\td = %2.4f\nf(%2.4f) = %2.4f, f(%2.4f) = %2.4f\n\n", 
		range_min, range_max, d, x1, pfun1(&x1), x2, pfun1(&x2));

	if (d < 0.0001)
		return x1;
	if (pfun1(&x2) > pfun1(&x1))
	{
		range_max = x2;
//  		x2 = x1;
		F(pfun1, range_min, range_max);
	}
	else
	{
		range_min = x1;
//  		x1 = x2;
		F(pfun1, range_min, range_max);
	}
}

int main(int argc, char* argv[])
{
// 	ClassA CLASS;	//=ClassA=
	
	CBasicOptimizer A;
	enum LineSearch line = GoldenSection;
	A.SetLineSearchMethod(line);

// 	line = A.GetLineSearchMethod();
// 	printf("..LineSearchMethod = %d\n", line);


	double d, range_max, range_min;

	range_max = RANGE_MAX;
	range_min = RANGE_MIN;

	double ans = F(pfun1, range_min, range_max);
 	printf("Ans = %2.4f\n\n", ans);

// 	double x = 0.3;
// 	double ans = pfun1(&x);
// 	double Answer = A.OptOneDimensionFunSolver(pfun1, x);
// 	printf("input = %g\n\nans = %g\n\n",x, Answer);

	return 0;
}

