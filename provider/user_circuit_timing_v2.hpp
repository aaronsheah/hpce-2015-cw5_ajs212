#ifndef user_circuit_timing_hpp
#define user_circuit_timing_hpp

#include "puzzler/puzzles/circuit_timing.hpp"

#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"

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
	    	
	      std::vector<unsigned> delays(input->flipFlopCount,0);
	      auto timing_analysis = [&](unsigned i) {
			delays[i]=delay(input->wires.at(i), input);
	      };
	      tbb::parallel_for<unsigned>(0, input->flipFlopCount, timing_analysis);

	      for (unsigned i = 0; i < input->flipFlopCount; i++) {
	      		histogram[delays[i]]++;
	      }

	      log->LogVerbose("Finished timing analysis, converting histogram");
	      
	      std::vector<uint32_t> res(histogram.rbegin()->first+1, 0);

	      for(unsigned i=0; i<res.size(); i++){
			if(histogram.find(i)!=histogram.end()){
				res.at(i)=histogram.at(i);
			}
			log->LogInfo(" histogram[%u] = %u", i, res[i]);
		}
	      
	      output->criticalPathHistogram=res;
	      
	      log->LogVerbose("Finished converting histogram");
	}
};

#endif
