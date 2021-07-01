#include <iostream>
#include <gtest/gtest.h>
#include <memory>
#include <cassert>

#include "pata_utils.h"
#include "pata_tensor.h"
#include "pata_operator.h"
#include "pata_backend.h"

using namespace libpata;

TEST(TensorOperationTests, TestReluN) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    auto x = reluN(t);
    BackendManager::Inst().backend()->wait_for_all();
}

TEST(TensorOperationTests, TestAbs) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    auto x = abs(t);
    BackendManager::Inst().backend()->wait_for_all();
}

TEST(TensorOperationTests, TestAdd1) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({20, 30}, FLOAT, ws);
    float *ptF = (float*)t.base_addr();
    for (unsigned i=0; i<t.volume(); i++) ptF[i]=i*0.1;
    Tensor s1 = t.subrange(Range(2), Range(0, 10));
    Tensor s2 = t[{Range(2), Range(5, 15)}];
    ASSERT_EQ(*s1.at<float>(1,1), 3.1f);
    ASSERT_EQ(*s2.at<float>(1,1), 3.6f);

    EXPECT_EQ(s1.shape(), s2.shape());
    auto x = s1 + s2;    
    //StreamManager::Inst().wait_for_all();
    ASSERT_EQ(*x.at<float>(1,1), 6.7f);
    BackendManager::Inst().backend()->wait_for_all();
}

TEST(TensorOperationTests, TestMauMul1Tile) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor a({20, 30}, FLOAT, ws);
    a.fill(0.0f, 0.25f);
    Tensor b({30, 20}, FLOAT, ws);
    b.fill(10.0f, 1.0f);
    Tensor out({20, 20}, FLOAT, ws);

    TensorsList out_tiles;
    MatMul(a, b, out, out_tiles);
    
}
