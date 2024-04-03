#include "pch.h"
struct Net : torch::nn::Module {
  Net(int64_t N, int64_t M) {
    W = register_parameter("W", torch::randn({N, M}));
    b = register_parameter("b", torch::randn(M));
  }
  torch::Tensor forward(torch::Tensor input) {
    return torch::addmm(b, input, W);
  }
  
  torch::Tensor W, b;
};

int main(int argc, char* argv[]) {

  // torch::DeviceType device_type = at::kCPU;

  if (torch::cuda::is_available()) {
    std::cout << "Torch : CUDA is available!" << std::endl;
  } else {
    std::cout << "Torch : Use CPU only." << std::endl;
  }

  torch::Tensor tensor = torch::rand({4, 4}, torch::kCUDA);
  std::cout << "Torch::Tensor =\n" << tensor << std::endl;

  return EXIT_SUCCESS;
}
