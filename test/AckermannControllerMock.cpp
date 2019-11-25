/*
 * @file AckermannControllerMock.hpp
 * @Author Gautam Balachandran
 * Created on 24 November 2019
 * @brief Ackermann Controller GMock test file
 */

/*
 The MIT License
 Copyright 2019 Gautam Balachandran, Sri Sai Kaushik, Sri Manika Makam

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <gmock/gmock.h>
#include <vector>
#include <AckermannController.h>

using ::testing::_;

class AckermannControllerMock : public AckermannController{
 public:
  /*
   * @brief : The given function mocks the method getTheta 
   * 
   * @param none
   * @return void
   */
  MOCK_METHOD0(getTheta, double);
  /*
   * @brief : The given function mocks the method computeLH 
   * 
   * @param none
   * @return void
   */
  MOCK_METHOD1(computeLH, double);
  /*
   * @brief : The given function mocks the method controlConstants 
   * 
   * @param none
   * @return void
   */
  MOCK_METHOD2(controlConstants, std::vector<double>);
};
/**
 * @brief Test case to test that steering angle
 * is calculated properly
 * @param none
 * @return none
 */
TEST(ackermannControllerTest, steeringTest) {
  AckermannControllerMock controllerMock;
  ::testing::Expectation init = EXPECT_CALL(controllerMock,
                                            computeLH(_)).Times(1);
  ::testing::Expectation init = EXPECT_CALL(controllerMock,
                                            controlConstants(_)).Times(1);
  EXPECT_CALL(controllerMock, getTheta(_)).After(init);
  std::vector<double> testD{20.5, 30.2};
  double testTheta = 30.0;
  mock.computeSteering(testD, testTheta);
}
