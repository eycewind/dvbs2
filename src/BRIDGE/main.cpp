#include <cmath>
#include <vector>
#include <iostream>

#include <aff3ct.hpp>

#include "Scrambler/Scrambler_BB/Scrambler_BB.hpp"
#include "Scrambler/Scrambler_PL/Scrambler_PL.hpp"
#include "Params_DVBS2O/Params_DVBS2O.hpp"
#include "Framer/Framer.hpp"
#include "Filter/Filter_UPFIR/Filter_UPRRC/Filter_UPRRC_ccr_naive.hpp"
#include "Sink/Sink.hpp"
#include "Factory_DVBS2O/Factory_DVBS2O.hpp"
#include "Estimator/Estimator.hpp"

using namespace aff3ct;

int main(int argc, char** argv)
{
	const auto params = Params_DVBS2O(argc, argv);

	if(params.section == "section1")
	{
		std::cout << "LOL1" << std::endl;
		// buffers to load/store the data
		std::vector<float> matlab_input (10);
		std::vector<int>   matlab_output(10);

		// construct tools
		Sink sink_to_matlab(params.mat2aff_file_name, params.aff2mat_file_name);

		sink_to_matlab.pull_vector(matlab_input);

		sink_to_matlab.push_vector(matlab_output, false);
	}
	else if (params.section == "section2")
	{
		std::cout << "LOL2" << std::endl;
	}
	else
	{
		std::cout << "LOL3" << std::endl;
	}
	

	return EXIT_SUCCESS;
}