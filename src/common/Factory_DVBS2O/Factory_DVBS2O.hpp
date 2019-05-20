#ifndef FACTORY_DVBS2O_HPP
#define FACTORY_DVBS2O_HPP

#include <aff3ct.hpp>

#include "../Params_DVBS2O/Params_DVBS2O.hpp"
#include "../Framer/Framer.hpp"
#include "../Scrambler/Scrambler_BB/Scrambler_BB.hpp"
#include "../Scrambler/Scrambler_PL/Scrambler_PL.hpp"
#include "../Filter/Filter_UPFIR/Filter_UPRRC/Filter_UPRRC_ccr_naive.hpp"
#include "../Estimator/Estimator.hpp"

struct Factory_DVBS2O {
	template <typename B = int>
	static module::Source<B>* build_source(Params_DVBS2O& params);
	
	template <typename B = int>
	static module::Encoder_BCH<B>* build_bch_encoder(Params_DVBS2O& params,
	                                                 tools::BCH_polynomial_generator<B>& poly_gen);
	
	template <typename B = int, typename Q = float>
	static module::Decoder_BCH_std<B,Q>* build_bch_decoder(Params_DVBS2O& params,
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

	template <typename B = int>
	static module::Scrambler_BB<B>* build_bb_scrambler(Params_DVBS2O& params);

	template <typename R = float>
	static module::Scrambler_PL<R>* build_pl_scrambler(Params_DVBS2O& params);

	template <typename R = float>
	static module::Filter_UPRRC_ccr_naive<R>* build_uprrc_filter(Params_DVBS2O& params);

	template <typename R = float>	
	static module::Estimator<R>* build_estimator(Params_DVBS2O& params);

	template <typename B = int>
	static module::Monitor_BFER<B>* build_monitor(Params_DVBS2O& params);

};

#endif /* FACTORY_DVBS2O_HPP */
