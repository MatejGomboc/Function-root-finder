#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>


inline int sign(const double value)
{
	return (value >= 0.0) ? 1 : -1;
}


bool find_root(double& root, double low, double high, gsl_function& function, const double error = 1e-10, const int max_iter = 100)
{
	if (sign(function.function(low, nullptr)) != sign(function.function(high, nullptr)))
	{
		gsl_set_error_handler_off();

		int status;
		int iter = 0;

		gsl_root_fsolver* solver = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
		if (solver == nullptr) throw std::exception(("GSL error (" + std::string(__FILE__) + "," + std::to_string(__LINE__) + "): gsl_root_fsolver_alloc.").c_str());

		status = gsl_root_fsolver_set(solver, &function, low, high);
		if (status != GSL_SUCCESS)
		{
			throw std::exception(("GSL error(" + std::string(__FILE__) + ", " + std::to_string(__LINE__) + ") : gsl_root_fsolver_set, " + std::string(gsl_strerror(status)) + ".").c_str());
		}

		do
		{
			iter++;

			status = gsl_root_fsolver_iterate(solver);
			if (status != GSL_SUCCESS)
			{
				throw std::exception(("GSL error (" + std::string(__FILE__) + "," + std::to_string(__LINE__) + "): gsl_root_fsolver_iterate, " + std::string(gsl_strerror(status)) + ".").c_str());
			}

			root = gsl_root_fsolver_root(solver);
			low = gsl_root_fsolver_x_lower(solver);
			high = gsl_root_fsolver_x_upper(solver);
			status = gsl_root_test_interval(low, high, error, 0.0);

		} while ((status == GSL_CONTINUE) && (iter < max_iter));

		gsl_root_fsolver_free(solver);
		gsl_set_error_handler(NULL);
		return true;
	}
	return false;
}


double test_function(double x, void* params)
{
	// f(x) = x^3 - 5x^2 + 8x - 4
	return x * x * x - 5.0 * x * x + 8.0 * x - 4.0;
}


int main(void)
{
	try
	{
		for (double low = 0.1; low < 10.0; low += 1.0)
		{
			double root = low;
			double high = low + 1.0;

			if (find_root(root, low, high, gsl_function{ test_function, nullptr }, 1e-5))
			{
				std::cout << "root: " << root << ", low: " << low << ", high: " << high << std::endl;
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
