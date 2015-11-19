#ifndef user_brute_force_hpp
#define user_brute_force_hpp

#include "puzzler/puzzles/brute_force.hpp"

#include "tbb/parallel_for.h"

#define __CL_ENABLE_EXCEPTIONS 
#include "CL/cl.hpp"
#include <fstream>
#include <streambuf>

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

void kernel (
    uint i,
    const uint *input_key, const uint input_rounds, const ulong input_cipherText,
    int *hit
  ) {
    ulong tmp=TEA_hash(i, input_key, input_rounds);
    hit[i] = tmp==input_cipherText;
}

std::string LoadSource(const char *fileName)
{
    // TODO : Don't forget to change your_login here
    std::string baseDir="provider";
    if(getenv("HPCE_CL_SRC_DIR")){
        baseDir=getenv("HPCE_CL_SRC_DIR");
    }

    std::string fullName=baseDir+"/"+fileName;

    // Open a read-only binary stream over the file
    std::ifstream src(fullName, std::ios::in | std::ios::binary);
    if(!src.is_open())
        throw std::runtime_error("LoadSource : Couldn't load cl file from '"+fullName+"'.");

    // Read all characters of the file into a string
    return std::string(
        (std::istreambuf_iterator<char>(src)), // Node the extra brackets.
        std::istreambuf_iterator<char>()
    );
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
      
      /********* Original *********/
      // for(unsigned i=0; i<input->n; i++){
      //   uint64_t tmp=TEA_hash(i, &input->key[0], input->rounds);
      //   hit[i] = tmp==input->cipherText;
      //   log->LogDebug("  encrypt(%llu) == %llu, hit=%i", (uint64_t)i, tmp, hit[i]);
      // }
      
      /********* TBB Parfor *********/
      auto loop_body = [&] (unsigned i){
        uint64_t tmp=TEA_hash(i, &input->key[0], input->rounds);
        hit[i] = tmp==input->cipherText;
        log->LogDebug("  encrypt(%llu) == %llu, hit=%i", (uint64_t)i, tmp, hit[i]);
      };
      tbb::parallel_for<unsigned>(0,input->n, loop_body);

      /********* OpenCL V1 *********/
     //  auto kernel = [&] (unsigned i) {
     //    uint64_t tmp=TEA_hash(i, &input->key[0], input->rounds);
     //    hit[i] = tmp==input->cipherText;
     //    log->LogDebug("  encrypt(%llu) == %llu, hit=%i", (uint64_t)i, tmp, hit[i]);
     //  };

     // for(unsigned i=0; i<input->n; i++){
     //    kernel(i);  
     //  }

      /********* OpenCL V2 *********/
     // for(unsigned i=0; i<input->n; i++){
     //    kernel(i, &input->key[0], input->rounds, input->cipherText, (int*) &hit[0]); 
     //  }

     /********* OpenCL V3 *********/

      // // Chosing a platform
      // std::vector<cl::Platform> platforms;

      // cl::Platform::get(&platforms);
      // if(platforms.size()==0)
      //   throw std::runtime_error("No OpenCL platforms found.");

      // int selectedPlatform=0;
      // if(getenv("HPCE_SELECT_PLATFORM")){
      //   selectedPlatform=atoi(getenv("HPCE_SELECT_PLATFORM"));
      // }
      // cl::Platform platform=platforms.at(selectedPlatform);   

      // // Chosing a device
      // std::vector<cl::Device> devices;
      // platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);  
      // if(devices.size()==0){
      //   throw std::runtime_error("No opencl devices found.\n");
      // }

      // // Creating a Context
      // cl::Context context(devices);

      // std::string kernelSource=LoadSource("user_brute_force_kernel.cl");

      // cl::Program::Sources sources;   // A vector of (data,length) pairs
      // sources.push_back(std::make_pair(kernelSource.c_str(), kernelSource.size()+1)); // push on our single string

      // cl::Program program(context, sources);

      // try{
      //   program.build(devices);
      // }catch(...){
      //   for(unsigned i=0;i<devices.size();i++){
      //     std::cerr<<"Log for device "<<devices[i].getInfo<CL_DEVICE_NAME>()<<":\n\n";
      //     std::cerr<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i])<<"\n\n";
      //   }
      //   throw;
      // }

      // // Allocating Buffers
      // size_t cbBuffer=input->n * 4;
      // cl::Buffer buffKeys(context, CL_MEM_READ_ONLY, cbBuffer);
      // cl::Buffer buffHit(context, CL_MEM_READ_ONLY, cbBuffer);

      // // Setting kernel parameters
      // cl::Kernel kernel(program, "kernelBrute");

      // kernel.setArg(0, buffKeys);
      // kernel.setArg(1, input->rounds);
      // kernel.setArg(2, input->cipherText);
      // kernel.setArg(3, buffHit);

      // // Creating a command queue 
      // cl::CommandQueue queue(context,devices[0]);

      // // Copying over fixed data
      // queue.enqueueWriteBuffer(buffKeys, CL_TRUE, 0, cbBuffer, &input->key[0]);

      // cl::NDRange offset(0);               // Always start iterations at x=0, y=0
      // cl::NDRange globalSize(input->n); 
      // cl::NDRange localSize=cl::NullRange;    // We don't care about local size

      // // Execute Kernel
      // cl::Event evExecutedKernel;
      // queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize, NULL, &evExecutedKernel);
      
      // // Read output
      // std::vector<cl::Event> copyBackDependencies(1, evExecutedKernel);
      // queue.enqueueReadBuffer(buffHit, CL_TRUE, 0, cbBuffer, &hit[0], &copyBackDependencies);

      /*****************************/

      uint32_t index=std::find(hit.begin(), hit.end(), 1)-hit.begin();
      if(index>=hit.size())
        throw std::runtime_error("Plain-text not found.");
      
      output->plainText = index;
  }

  

};

#endif
