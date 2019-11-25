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
  ::testing::Expectation init = EXPECT_CALL(controllerMock, computeLH(_)).Times(1);
  ::testing::Expectation init = EXPECT_CALL(controllerMock, controlConstants(_)).Times(1);
  EXPECT_CALL(controllerMock, getTheta(_)).After(init);
  
  std::vector<double> testD{20.5,30.2};
  double testTheta = 30.0;
  mock.computeSteering(testD, testTheta);
}