#ifndef FACTORY_DVBS2O_HPP
#define FACTORY_DVBS2O_HPP

#include <aff3ct.hpp>

#include "../Params_DVBS2O/Params_DVBS2O.hpp"
#include "../Framer/Framer.hpp"
struct Factory_DVBS2O {
	template <typename B = int>
	static module::Encoder_BCH<B>* build_bch_encoder(Params_DVBS2O& params,
	                                                 tools::BCH_polynomial_generator<B>& poly_gen);

	template <typename B = int, typename Q = float>
	static module::Codec_LDPC<B,Q>* build_ldpc_cdc(Params_DVBS2O& params);

	template <typename D = uint32_t>
	static tools::Interleaver_core<D>* build_itl_core(Params_DVBS2O& params);

	template <typename D = int32_t, typename T = uint32_t>
	static module::Interleaver<D,T>* build_itl(Params_DVBS2O& params, 
	                                           tools::Interleaver_core<T>& itl_core);

	template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX = tools::max_star>
	static module::Modem_generic<B,R,Q,MAX>* build_modem(Params_DVBS2O& params,
	                                                     std::unique_ptr<tools::Constellation<R>> cstl);
	template <typename R = float>
	static module::Framer<R>* build_framer(Params_DVBS2O& params);
};

#endif /* FACTORY_DVBS2O_HPP */