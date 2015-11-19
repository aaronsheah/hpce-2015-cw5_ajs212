#ifndef user_circuit_timing_hpp
#define user_circuit_timing_hpp

#include "puzzler/puzzles/circuit_timing.hpp"


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
	      
	      for(unsigned i=0; i<input->flipFlopCount; i++){
	        unsigned got=delay(i, input);
	        histogram[got]++;
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
