#include <iostream>
#include <gtest/gtest.h>
#include <memory>

#include "tosa_utils.h"
#include "tosa_tensor.h"
#include "tosa_operator.h"

using namespace libtosa;
// Demonstrate some basic assertions.
TEST(TensorTests, StrideTest) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    Shape ex_stride = {600,30,1};
    EXPECT_EQ(t.stride(), ex_stride);
}

TEST(TensorTests, TestView1) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);

    for (int i=0; i<10 ; i++)
        for (int j=0; j<20 ; j++)
            for (int k=0; k<20 ; k++)
            {
                *t.at<float>(i,j,k) = i*10000.+j*100.+k;
            }
    Tensor s1 = t[{Range(5, 15), Range(15), Range(10, 20, 2)}];
    Tensor s2 = t.subrange(Range(5, 15, 2));
    EXPECT_EQ(s1.stride(), Shape({600,30,2}));
    EXPECT_EQ(s1.shape(), Shape({5,15,5}));
    float *pf = s1.at<float>(0,5,1);
    EXPECT_EQ(50512. , *pf);
    
    EXPECT_EQ(50514. , *s1.at<float>(0,5,2));

    EXPECT_EQ(s2.stride(), Shape({1200,30,1}));
    EXPECT_EQ(s2.shape(), Shape({3,20,30}));
    EXPECT_EQ(70501. , *s2.at<float>(1,5,1));
    EXPECT_EQ(90502. , *s2.at<float>(2,5,2));

}

TEST(UtilsTests, TestListRef) {
    auto ws = std::make_shared<Workspace>(1000000);
    std::list<std::weak_ptr<TensorImpl>> a_list;
    std::list<WeakListReference<TensorImpl>> refernces;
    std::shared_ptr<TensorImpl> t(new TensorImpl({10, 20, 30}, FLOAT, ws));
    refernces.emplace_back(&a_list, t);
    refernces.clear();
    EXPECT_TRUE(a_list.empty());    
}

TEST(UtilsTests, TestTensorOverlap) {
 auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    Tensor s1 = t.subrange(Range(1), Range(0, 10));
    Tensor s2 = t[{Range(1), Range(5, 15)}];
    auto signal = std::make_shared<Signal>("test_sig1");
    s1.set_signal(signal);
    ASSERT_FALSE(s2.is_ready());
    ASSERT_FALSE(t.is_ready());
}

TEST(UtilsTests, TestNonTensorOverlap) {
 auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    Tensor s1 = t.subrange(Range(1), Range(0, 5));
    Tensor s2 = t[{Range(1), Range(5, 15)}];
    auto signal = std::make_shared<Signal>("test_sig1");
    s1.set_signal(signal);
    ASSERT_TRUE(s2.is_ready());
    ASSERT_FALSE(t.is_ready());
}


TEST(UtilsTests, TestNonTensorOverlapInterleaved) {
 auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    Tensor s1 = t.subrange(Range(1), Range(0, 10, 2));
    Tensor s2 = t[{Range(1), Range(5, 15, 2)}];
    auto signal = std::make_shared<Signal>("test_sig1");
    s1.set_signal(signal);
    ASSERT_TRUE(s2.is_ready());
    ASSERT_FALSE(t.is_ready());
}


TEST(TensorOperationTests, TestReluN) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    auto x = reluN(t);
}

TEST(TensorOperationTests, TestAbs) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    auto x = abs(t);
}


TEST(TensorOperationTests, TestAdd1) {
    auto ws = std::make_shared<Workspace>(1000000);
    Tensor t({10, 20, 30}, FLOAT, ws);
    Tensor s1 = t.subrange(Range(1), Range(0, 10));
    Tensor s2 = t[{Range(1), Range(5, 15)}];

    EXPECT_EQ(s1.shape(), s2.shape());
    auto x = s1 + s2;
}