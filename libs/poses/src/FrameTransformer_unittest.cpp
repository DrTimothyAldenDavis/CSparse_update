/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2022, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include <gtest/gtest.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/FrameTransformer.h>

#include <Eigen/Dense>

template <int DIM>
void run_tf_test1(const mrpt::poses::CPose2D& A2B_)
{
	using namespace mrpt::poses;

	FrameTransformer<DIM> tf;

	// Pub:
	const auto real_A2B = (typename FrameTransformer<DIM>::pose_t)(A2B_);
	tf.sendTransform("A", "B", real_A2B);

	// Read:
	{
		typename FrameTransformer<DIM>::pose_t A2B_looked_up;
		const auto ret = tf.lookupTransform("B", "A", A2B_looked_up);
		EXPECT_EQ(ret, mrpt::poses::LKUP_GOOD);
		EXPECT_NEAR(
			.0,
			(real_A2B.asVectorVal() - A2B_looked_up.asVectorVal())
				.array()
				.abs()
				.sum(),
			1e-6);
	}
}

TEST(FrameTransformer, SimplePublishAndLookup)
{
	const mrpt::poses::CPose2D test_A2B(5.0, 2.0, 0.3);

	run_tf_test1<2>(test_A2B);
	run_tf_test1<3>(test_A2B);
}
