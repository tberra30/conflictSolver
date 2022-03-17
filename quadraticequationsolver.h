#ifndef QUADRATICEQUATIONSOLVER_H
#define QUADRATICEQUATIONSOLVER_H


class QuadraticEquationSolver
{
public:
    QuadraticEquationSolver(double a, double b, double c);
    bool getHasSolution();
    double getR1();
    double getR2();

private:
    double r1;
    double r2;
    bool hasSolution;

};

#endif // QUADRATICEQUATIONSOLVER_H
