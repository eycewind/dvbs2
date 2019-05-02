/*!
 * \file
 * \brief Generates a message.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SCRAMBLER_HXX_
#define SCRAMBLER_HXX_

#include <sstream>
#include <complex>

#include "Tools/Exception/exception.hpp"

#include "Scrambler.hpp"

namespace aff3ct
{
namespace module
{

template <typename D>
Scrambler<D>::
Scrambler(const int N, const int n_frames)
: Module(n_frames), N(N)
{
	const std::string name = "Scrambler";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
	
	auto &p1 = this->create_task("scramble");
	auto &p1s_X_N1 = this->template create_socket_out<D>(p1, "X_N1", this->N * this->n_frames);
	auto &p1s_X_N2 = this->template create_socket_out<D>(p1, "X_N2", this->N * this->n_frames);
	this->create_codelet(p1, [this, &p1s_X_N1, &p1s_X_N2]() -> int
	{
		this->scramble(static_cast<D*>(p1s_X_N1.get_dataptr()),
		               static_cast<D*>(p1s_X_N2.get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("descramble");
	auto &p2s_Y_N1 = this->template create_socket_in <D>(p2, "Y_N1", this->N * this->n_frames);
	auto &p2s_Y_N2 = this->template create_socket_out<D>(p2, "Y_N2", this->N * this->n_frames);
	this->create_codelet(p2, [this, &p2s_Y_N1, &p2s_Y_N2]() -> int
	{
		this->descramble(static_cast<D*>(p2s_Y_N1.get_dataptr()),
		                 static_cast<D*>(p2s_Y_N2.get_dataptr()));

		return 0;
	});
}

template <typename D>
int Scrambler<D>::
get_N() const
{
	return N;
}

template <typename D>
template <class A>
void Scrambler<D>::
scramble(std::vector<D,A>& X_N1, std::vector<D,A>& X_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)X_N1.size())
	{
		std::stringstream message;
		message << "'X_N1.size()' has to be equal to 'N' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
	if (this->N * this->n_frames != (int)X_N2.size())
	{
		std::stringstream message;
		message << "'X_N2.size()' has to be equal to 'N' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->scramble(X_N1.data(), X_N2.data(), frame_id);
}

template <typename D>
void Scrambler<D>::
scramble(D *X_N1, D *X_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_scramble(X_N1 + f * this->N, X_N2 + f * this->N, f);
}

template <typename D>
template <class A>
void Scrambler<D>::
descramble(std::vector<D,A>& Y_N1, std::vector<D,A>& Y_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->scramble(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename D>
void Scrambler<D>::
descramble(D *Y_N1, D *Y_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_scramble(Y_N1 + f * this->N, Y_N2 + f * this->N, f);
}

template <typename D>
void Scrambler<D>::
_scramble(D *X_N1, D *X_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename D>
void Scrambler<D>::
_descramble(D *Y_N1, D *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}



}
}

#endif
