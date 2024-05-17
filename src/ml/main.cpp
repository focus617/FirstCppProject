#include "pch.h"

#include <torch/torch.h>
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
  std::cout << "PyTorch version: " 
            << TORCH_VERSION_MAJOR << "."
            << TORCH_VERSION_MINOR << "." 
            << TORCH_VERSION_PATCH << std::endl;

  torch::DeviceType device_type = at::kCPU;

  if (torch::cuda::is_available()) {
    std::cout << "Torch : CUDA is available!" << std::endl;
    std::cout << "And " << torch::cuda::device_count() << " GPU is available!" << std::endl;
    device_type = at::kCUDA;
  } else {
    std::cout << "Torch : Use CPU only." << std::endl;
  }
  std::cout << "device_type : "<< device_type << std::endl;

  // scalar
  torch::Tensor scalar = torch::tensor(7);
  std::cout << "scalar = " << scalar << "\t";
  std::cout << "with value (" << scalar.item() << ") and ";
  std::cout << "dimention (" << scalar.dim() << ")";
  std::cout << std::endl << std::endl;

  // vector
  torch::Tensor vector = 
    torch::tensor({1, 2, 3}, torch::dtype(torch::kFloat32));
  std::cout << "vector = \n" << vector << "\t";
  std::cout << "with dimention (" << vector.dim() << ") and ";
  std::cout << "shape (" << vector.sizes() << ")";
  std::cout << std::endl << std::endl;

  // matrix
  torch::Tensor MATRIX = torch::rand({4, 4}, torch::kCPU);
  std::cout << "MATRIX (in CPU) =\n" << MATRIX << "\t";
  std::cout << "with dimention (" << MATRIX.dim() << "), ";
  std::cout << "shape (" << MATRIX.sizes() << "), ";
  std::cout << "device (" << MATRIX.device() << ")";
  std::cout << std::endl << std::endl;

  // tensor
  torch::Tensor TENSOR = torch::randn({2, 3, 3}, device_type);
  std::cout << "TENSOR =\n" << TENSOR << "\t";
  std::cout << "with dimention (" << TENSOR.dim() << "), ";
  std::cout << "shape (" << TENSOR.sizes() << "), ";
  std::cout << "device (" << TENSOR.device() << ")";
  std::cout << std::endl << std::endl;

  std::cout << "TENSOR[0] =\n" << TENSOR[0] << std::endl << std::endl;

  // Create a range of values 0 to 10
  auto zero_to_ten = torch::arange(0, 10, 1);
  std::cout << zero_to_ten << std::endl;

  auto ten_zeros = torch::zeros_like(zero_to_ten);
  std::cout << ten_zeros << std::endl;

  /* 
  An instance of TensorOptions stores a concrete value for each of these axes. Here is an example of creating a TensorOptions object that represents a 64-bit float, strided tensor that requires a gradient, and lives on CUDA device 0:
  */
  auto options = torch::TensorOptions()
    .dtype(torch::kFloat32)
    .layout(torch::kStrided)
    .device(torch::kCUDA, 0)
    .requires_grad(true);

  torch::Tensor tensor = torch::full({3, 4}, /*value=*/123, options);
  std::cout << tensor << std::endl;

  assert(tensor.dtype() == torch::kFloat32);
  assert(tensor.layout() == torch::kStrided);
  assert(tensor.device().type() == torch::kCUDA); // or device().is_cuda()
  assert(tensor.device().index() == 0);
  assert(tensor.requires_grad());

  torch::Tensor t = 
    torch::tensor({1, 2, 3}, torch::dtype(torch::kFloat32));

  torch::Tensor matrix_multiplication = torch::matmul(t, t);
  std::cout << "torch::matmul(" << t << " * " << t 
            << ") = " << matrix_multiplication << std::endl << std::endl;


  // Create a range of values 0 to 90
  auto zero_to_ninty = torch::arange(0, 100, 10);
  std::cout << zero_to_ninty << std::endl;
  std::cout << "min = " << zero_to_ninty.min() << std::endl;       //求最大值
  std::cout << "max = " << torch::max(zero_to_ninty) << std::endl; //求最小值
  std::cout << "Index where max value occurs: " << zero_to_ninty.argmax()<< std::endl;
  std::cout << "Index where min value occurs: " << torch::argmin(zero_to_ninty)<< std::endl;

  std::cout << "sum = " << zero_to_ninty.sum() << std::endl;
  std::cout << "mean = " << zero_to_ninty.to(torch::kFloat32).mean() << std::endl;

  at::Tensor a = torch::rand({ 2, 4 });
  std::cout << a << std::endl;
 
  // 求a第0维度的最大值
  std::tuple<torch::Tensor, torch::Tensor> max_classes = (torch::max)(a, 0);   
  auto max_val = std::get<0>(max_classes);    //求得最大值
  auto max_index = std::get<1>(max_classes);  //求得最大值的索引
  std::cout << max_val << std::endl;
  std::cout << max_index << std::endl;
   
  // 求a第1维度的最小值
  auto min_classes = (torch::min)(a, 1);
  auto min_val = std::get<0>(min_classes);    //求得最小值
  auto min_index = std::get<1>(min_classes);  //求得最小值的索引
  std::cout << min_val << std::endl;
  std::cout << min_index << std::endl;

  return EXIT_SUCCESS;
}
