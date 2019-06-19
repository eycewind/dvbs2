#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

#include "Synchronizer_LR_cc_naive.hpp"

// _USE_MATH_DEFINES does not seem to work on MSVC...
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace aff3ct::module;

template <typename R>
Synchronizer_LR_cc_naive<R>
::Synchronizer_LR_cc_naive(const int N, const std::vector<R> pilot_values, const std::vector<int> pilot_start)
: Synchronizer<R>(N,N), pilot_size(pilot_values.size()), pilot_nbr(pilot_start.size()), pilot_values(pilot_values), pilot_start(pilot_start), R_l(2,(R)0.0)
{
	assert(pilots_sz > 0);
	assert(pilot_size > 0);
	assert(N > pilot_start[pilot_nbr-1] + pilots_sz - 1);
}

template <typename R>
Synchronizer_LR_cc_naive<R>
::~Synchronizer_LR_cc_naive()
{}

template <typename R>
void Synchronizer_LR_cc_naive<R>
::_synchronize(const R *X_N1, R *Y_N2, const int frame_id)
{
	int P = this->pilot_nbr;
	int Lp = this->pilot_size/2;
	
	for  (int p = 0 ; p<P ; p++)
	{
		//std::cout << this->pilot_start[p] << std::endl;
		std::vector<R> z(2*Lp, (R)0.0);
		for (int i = 0 ; i<Lp ; i++)
		{
			z[2*i]     = X_N1[2*i +   + 2*this->pilot_start[p]] * pilot_values[2*i] 
			           + X_N1[2*i + 1 + 2*this->pilot_start[p]] * pilot_values[2*i + 1];

			z[2*i + 1] = X_N1[2*i + 1 + 2*this->pilot_start[p]] * pilot_values[2*i] 
			           - X_N1[2*i     + 2*this->pilot_start[p]] * pilot_values[2*i + 1];

					   //std::cout << "(" << X_N1[2*i +   + 2*this->pilot_start[p]] << " " << X_N1[2*i + 1  + 2*this->pilot_start[p]] << ")";
		}
		
		for (int m = 1 ; m < Lp/2 + 1 ; m++)
		{
			std::vector<float  > sum_xcorr_z(2, 0.0f);

			for (int k = m; k < Lp ; k++ )
			{
				sum_xcorr_z[0] += z[2*k]     * z[2*(k-m)] 
				                + z[2*k + 1] * z[2*(k-m) + 1];

				sum_xcorr_z[1] += z[2*k + 1] * z[2*(k-m)] 
				                - z[2*k]     * z[2*(k-m) + 1];
			}
			this->R_l[0] += sum_xcorr_z[0] / (R)(Lp-m);
			this->R_l[1] += sum_xcorr_z[1] / (R)(Lp-m);
		}
	}
	
	R est_reduced_freq = std::atan2(this->R_l[1], this->R_l[0]);
	est_reduced_freq /= (Lp/2 + 1) * M_PI;
	
	//std::cout << "# {INTERNAL} hat_nu = "<< est_reduced_freq << " " << std::endl;
	for (int n = 0 ; n < this->N_in/2 ; n++)
	{
		float t = (R)(2 * n) / (R)this->N_in;
		Y_N2[2*n]   = X_N1[2*n]     * std::cos( 2*M_PI * est_reduced_freq * n) 
		            + X_N1[2*n + 1] * std::sin( 2*M_PI * est_reduced_freq * n);

		Y_N2[2*n+1] = X_N1[2*n + 1] * std::cos( 2*M_PI * est_reduced_freq * n)
		            - X_N1[2*n]     * std::sin( 2*M_PI * est_reduced_freq * n);
	}
}

template <typename R>
void Synchronizer_LR_cc_naive<R>
::reset()
{
	this->R_l[0] = (R)0;
	this->R_l[1] = (R)0;
} 

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Synchronizer_LR_cc_naive<float>;
template class aff3ct::module::Synchronizer_LR_cc_naive<double>;
// ==================================================================================== explicit template instantiation