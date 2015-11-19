#ifndef user_brute_force_hpp
#define user_brute_force_hpp

#include "puzzler/puzzles/brute_force.hpp"

#include "tbb/parallel_for.h"

uint64_t TEA_hash (uint64_t v, const uint32_t *k, unsigned rounds) {
    uint32_t v0=v&0xFFFFFFFFull, v1=v>>32, sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    
    uint64_t res=1234567801234567ull;
    for (i=0; i < rounds; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        res=((res << 7) ^ (res>>57)) + (v0&v1); // Non-linear, oh-noes! 
    }     /* end cycle */
    return res;
  }

class BruteForceProvider
  : public puzzler::BruteForcePuzzle
{
public:
  BruteForceProvider()
  {}

  virtual void Execute(
		       puzzler::ILog *log,
		       const puzzler::BruteForceInput *input,
		       puzzler::BruteForceOutput *output
		       ) const override {
    // return ReferenceExecute(log, input, output);
      log->LogInfo("Starting search, n=%u, cipherText=%llu, rounds=%u", input->n, input->cipherText, input->rounds);

      std::vector<int> hit(input->n, 0); // Array to hold hit/miss
      
      for(unsigned i=0; i<input->n; i++){
        uint64_t tmp=TEA_hash(i, &input->key[0], input->rounds);
        hit[i] = tmp==input->cipherText;
        log->LogDebug("  encrypt(%llu) == %llu, hit=%i", (uint64_t)i, tmp, hit[i]);
      }

      uint32_t index=std::find(hit.begin(), hit.end(), 1)-hit.begin();
      if(index>=hit.size())
        throw std::runtime_error("Plain-text not found.");
      
      output->plainText = index;
  }

  

};

#endif
