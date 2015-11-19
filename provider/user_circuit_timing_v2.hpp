#ifndef user_circuit_timing_hpp
#define user_circuit_timing_hpp

#include "puzzler/puzzles/circuit_timing.hpp"

#include "tbb/parallel_for.h"

class CircuitTimingProvider
  : public puzzler::CircuitTimingPuzzle
{
public:
  CircuitTimingProvider()
  {}

	virtual void Execute(
			   puzzler::ILog *log,
			   const puzzler::CircuitTimingInput *input,
				puzzler::CircuitTimingOutput *output
			   ) const override
	{
		log->LogVerbose("Starting timing analysis");
      
	      std::map<unsigned,unsigned> histogram;
	      
	      auto timing_analysis = [&](unsigned i) {
	      		unsigned got=delay(i, input);
			histogram[got]++;
	      }
	      tbb::parallel_for<unsigned>(0, input->flipFlopCount, timing_analysis);
	      
	      log->LogVerbose("Finished timing analysis, converting histogram");
	      
	      std::vector<uint32_t> res(histogram.rbegin()->first+1, 0);

	      auto convert_histogram = [&](unsigned i) {
			if(histogram.find(i)!=histogram.end()){
				res.at(i)=histogram.at(i);
			}
			log->LogInfo(" histogram[%u] = %u", i, res[i]);
	      }
	      tbb::parallel_for<unsigned>(0, res.size(), convert_histogram);

	      
	      output->criticalPathHistogram=res;
	      
	      log->LogVerbose("Finished converting histogram");
	}
};

#endif
