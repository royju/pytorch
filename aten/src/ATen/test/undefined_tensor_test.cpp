#include <gtest/gtest.h>

#include <ATen/ATen.h>
#include <c10/core/UndefinedTensorImpl.h>
#include <string>

using namespace at;

TEST(TestUndefined, UndefinedTest) {
  manual_seed(123);

  // mainly test ops on undefined tensors don't segfault and give a reasonable errror message.
  Tensor und;
  Tensor ft = ones({1}, CPU(kFloat));

  std::stringstream ss;
  ss << und << std::endl;
  ASSERT_FALSE(und.defined());
  ASSERT_EQ(std::string("UndefinedType"), und.toString());

  ASSERT_ANY_THROW(und.strides());
  ASSERT_ANY_THROW(und.dim());
  ASSERT_ANY_THROW([]() { return Tensor(); }() = Scalar(5));
  ASSERT_ANY_THROW(und.add(und));
  ASSERT_ANY_THROW(und.add(ft));
  ASSERT_ANY_THROW(ft.add(und));
  ASSERT_ANY_THROW(und.add(5));
  ASSERT_ANY_THROW(und.mm(und));

  und.toType(und.dispatch_type());
  ASSERT_ANY_THROW(und.toType(ft.dispatch_type()));
  ASSERT_ANY_THROW(ft.toType(und.dispatch_type()));
  und.toType(ScalarType::Undefined);
  ASSERT_ANY_THROW(und.toType(ScalarType::Float));
  ASSERT_ANY_THROW(ft.toType(ScalarType::Undefined));

  // copy_
  ASSERT_ANY_THROW(und.copy_(und));
  ASSERT_ANY_THROW(und.copy_(ft));
  ASSERT_ANY_THROW(ft.copy_(und));

  und.toBackend(Backend::Undefined);
  ASSERT_ANY_THROW(und.toBackend(Backend::CPU));
  ASSERT_ANY_THROW(ft.toBackend(Backend::Undefined));

  Tensor to_move = ones({1}, CPU(kFloat));
  Tensor m(std::move(to_move));
  ASSERT_FALSE(to_move.defined());
  ASSERT_EQ(to_move.unsafeGetTensorImpl(), UndefinedTensorImpl::singleton());
}
