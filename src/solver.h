//
// Created by Zakhar Eliseev on 13.03.2022.
//

#ifndef OTUS_SOLVER_H
#define OTUS_SOLVER_H

#include <vector>
#include <cmath>
#include <iomanip>
namespace Math {
    double default_eps = 0.001;

    bool close_to_zero(double number, double eps = default_eps) {
        return abs(number) < eps;
    }

    std::vector<double> solve(double a, double b, double c) {

        if (!std::isfinite(a) ||
            !std::isfinite(b) ||
            !std::isfinite(c)) {
            throw std::invalid_argument("argument cannot be INF or NaN");
        }

        if (close_to_zero(a)) {
            throw std::invalid_argument("'a' cannot be equal to 0");
        }

        double d = b * b - 4 * a * c;
        std::vector<double> result;

        if (d < -default_eps) {
            return {};
        }

        if (close_to_zero(d)) {
            result.push_back((-b) / (2 * a));
            result.push_back((-b) / (2 * a));
        }

        if (!close_to_zero(d)) {
            double sqrt_d = sqrt(d);
            result.push_back((-b + sqrt_d) / (2 * a));
            result.push_back((-b - sqrt_d) / (2 * a));
        }

        return result;
    }


}

#endif //OTUS_SOLVER_H
