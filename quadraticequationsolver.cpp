#include "quadraticequationsolver.h"
#include <QtMath>
#include <QDebug>

QuadraticEquationSolver::QuadraticEquationSolver(double a, double b, double c)
{
    bool isQuadratic = (a != 0.0);
    bool isLinear = (!isQuadratic) && (b != 0.0);

    if (isQuadratic) {
        double delta = pow(b, 2) - 4 * a * c;
        hasSolution = (delta >= 0);

        // Check if equation have solutions
        if (delta >= 0) {
            r1 = qMin((sqrt(delta) - b) / (2 * a), (-sqrt(delta) - b) / (2 * a));
            r2 = qMax((sqrt(delta) - b) / (2 * a), (-sqrt(delta) - b) / (2 * a));
        } else {
            r1 = NAN;
            r2 = NAN;
        }
    } else if (isLinear) {
        r1 = - c / b;
        r2 = NAN;
    } else {
        r1 = NAN;
        r2 = NAN;
    }
}

bool QuadraticEquationSolver::getHasSolution() {
    return hasSolution;
}

double QuadraticEquationSolver::getR1() {
    return r1;
}

double QuadraticEquationSolver::getR2() {
    return r2;
}
