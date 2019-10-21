/*
 * @file AckermannController.cpp
 * @Author Mushty Sri Sai Kaushik
 * Created on 15 October 2019
 * @brief Ackermann Controller class implementation
 */

/*
 The MIT License
 Copyright 2019 Gautam Balachandran, Sri Sai Kaushik, Sri Manika Makam

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <AckermannController.h>

/**
 * @brief Getter method for the heading vector
 * @param  none
 * @return The heading vector containing the x and y coordinates of the heading
 */
std::vector<double> AckermannController::getD() {
  return AckermannController::d;
}
/**
 * @brief Setter method for the heading vector
 * @param  New heading vector to be set
 * @return none
 */
void AckermannController::setD(std::vector<double> newD) {
  AckermannController::d.swap(newD);
}
/**
 * @brief Getter method for the orientation
 * @param  none
 * @return The orientation of the vehicle
 */
double AckermannController::getTheta() {
  return AckermannController::theta;
}
/**
 * @brief Setter method for the orientation
 * @param  New orientation to be set
 * @return none
 */
void AckermannController::setTheta(double newTheta) {
  AckermannController::theta = newTheta;
}
/**
 * @brief Getter method for the velocity
 * @param  none
 * @return The current velocity of the vehicle
 */
double AckermannController::getV() {
  return AckermannController::v;
}
/**
 * @brief Setter method for the velocity
 * @param  New velocity to be set
 * @return none
 */
void AckermannController::setV(double newV) {
  AckermannController::v = newV;
}
/**
 * @brief Getter method for the wheel base distance
 * @param  none
 * @return The wheel base distance
 */
double AckermannController::getL() {
  return AckermannController::l;
}
/**
 * @brief Setter method for the wheel base distance
 * @param  New wheel base distance
 * @return none
 */
void AckermannController::setL(double newL) {
  AckermannController::l = newL;
}
/**
 * @brief Method to compute the look-ahead distance which depends on the current velocity
 * @param
 * @return Computed look-ahead distance
 */
double AckermannController::computeLH() {
  double vel, lH;
  // Set values for vmin and vmax in metres/second
  int vmin = 6.94, vmax = 20.83;
  vel = AckermannController::getV();
  // Calculating value of look-ahead distance in metres
  if (vel < vmin) {
    lH = 10.41;
  } else if (vel >= vmin && vel <= vmax) {
    lH = vel * 1.5;
  } else {
    lH = 31.25;
  }
  return lH;
}
/**
 * @brief Method to compute the controller gains
 * @param none
 * @return Vector of computed gains, kp and kd
 */
std::vector<double> AckermannController::controlConstants() {
  double vel, kP, kD;
  vel = AckermannController::getV();
  // Calculating values of kP and kD, which are dependent on velocity
  kP = pow((0.3383 / vel), 2);
  kD = (0.4 / vel);
  std::vector<double> k { kP, kD };
  return k;
}
/**
 * @brief Method to compute the steering angle
 * @param Required heading and required orientation
 * @return Computed steering angle
 */
double AckermannController::computeSteering(std::vector<double> newD,
                                            double newTheta) {
  std::ofstream myfile;
  // Creating a file to write the outputs
  myfile.open("Output.txt");
  std::vector<double> k, d;
  double lH, th, dE, xD, yD, thetaD, thetaE, thetaIncr, phi;
  k = AckermannController::controlConstants();
  lH = computeLH();
  d = AckermannController::getD();
  th = AckermannController::getTheta();
  // thetaE is the difference between current & required orientations
  thetaE = th - newTheta;
  // dE is distance between the current and desired positions
  dE = (d[1] - newD[1]) * cos(newTheta * M_PI / 180)
      - (d[0] - newD[0]) * sin(newTheta * M_PI / 180);
  // Estimating the trajectory path in small increments
  thetaIncr = static_cast<double>(fabs(thetaE / 3));
  // Loop will run till the position error (dE) is between -5 and 5
  while (-5 > dE || dE > 5) {
    double k1, a1, a2, inner;
    // Printing position and orientation errors in the file
    myfile << "Current Orientation : " << th << "\n";
    myfile << "Position Error : " << dE << "\n";
    myfile << "Orientation Error : " << thetaE << "\n";
    // Maximum steering angle constraint is 45 degrees
    k1 = (tan(45.0 * M_PI / 180.0) / l);
    // Calculating steering angle (phi) using non-linear control law
    a1 = 1
        - exp(
            (-k1
                * (sin(thetaE * M_PI / 180)
                    * (k[1] * tan(thetaE * M_PI / 180) + k[0] * dE)))
                / (sin(thetaE * M_PI / 180)
                    + lH * (pow(cos(thetaE * M_PI / 180), 4))
                        * (k[1] * tan(thetaE * M_PI / 180) + k[0] * dE)));
    a2 = 1
        + exp(
            (-k1
                * (sin(thetaE * M_PI / 180)
                    * (k[1] * tan(thetaE * M_PI / 180) + k[0] * dE)))
                / (sin(thetaE * M_PI / 180)
                    + lH * (pow(cos(thetaE * M_PI / 180), 4))
                        * (k[1] * tan(thetaE * M_PI / 180) + k[0] * dE)));
    inner = static_cast<double>(-k1 * l * pow(cos(thetaE * M_PI / 180), 3)
        * (a1 / a2));
    phi = atan(inner) * 180 / M_PI;
    // Printing steering angle for every iteration
    myfile << "PHI : " << phi << "\n";
    // Calculation of orientation of the trajectory at next set point
    if (th < newTheta) {
      thetaD = th + thetaIncr;
    } else {
      thetaD = th - thetaIncr;
    }
    // Calculating the coordinates of next set point in the trajectory
    if (d[0] < newD[0]) {
      xD = d[0] + lH * cos(th * M_PI / 180);
    } else if (d[0] > newD[0]) {
      xD = d[0] - lH * cos(th * M_PI / 180);
    }
    if (d[1] < newD[1]) {
      yD = d[1] + lH * sin(th * M_PI / 180);
    } else if (d[1] > newD[1]) {
      yD = d[1] - lH * sin(th * M_PI / 180);
    }
    // Update values of th, d, xD, yD
    th = thetaD;
    d[0] = xD;
    d[1] = yD;
    // Calculating position and orientation errors
    dE = -(d[0] - newD[0]) * sin(newTheta * M_PI / 180)
        + (d[1] - newD[1]) * cos(newTheta * M_PI / 180);
    thetaE = th - newTheta;
    /*if (((newTheta - 3) >= th || th >= (newTheta + 3))) {
     thetaE = thetaD - newTheta;
     }*/
    myfile
        << "=============================================================== \n";
  }
  // Print the outputs in the text file
  myfile << "DESIRED POSITION : " << newD[0] << "," << newD[1] << "\n";
  myfile << "FINAL POSITION ERROR: " << dE << "\n";
  myfile << "FINAL POSITION : " << xD << "," << yD << "\n\n";
  myfile << "DESIRED ORIENTATION : " << newTheta << "\n";
  myfile << "FINAL ORIENTATION ERROR: " << thetaE << "\n";
  myfile << "FINAL ORIENTATION : " << thetaD << "\n\n";
  myfile << "FINAL STEERING ANGLE : " << phi << "\n";
  myfile.close();
  // Printing the outputs in console
  std::cout << "DESIRED POSITION : " << newD[0] << "," << newD[1] << std::endl;
  std::cout << "FINAL POSITION ERROR: " << dE << std::endl;
  std::cout << "FINAL POSITION : " << xD << "," << yD << std::endl << std::endl;
  std::cout << "DESIRED ORIENTATION : " << newTheta << std::endl;
  std::cout << "FINAL ORIENTATION ERROR: " << thetaE << std::endl;
  std::cout << "FINAL ORIENTATION : " << thetaD << std::endl;
  std::cout << "FINAL STEERING ANGLE : " << phi << std::endl << std::endl;
  return phi;
}
/**
 * @brief Method to compute the wheel drive velocities
 * @param Required velocity and required orientation
 * @return Computed vector of the wheel velocities
 */
std::vector<double> AckermannController::driveVelocities(double reqV,
                                                         double reqTheta) {
  double l, wheelRadius, trackWidth, angularVelocity, turningRadius, ICRI,
      displacement1, rps1, driveVelocity1, ICRO, displacement2, rps2,
      driveVelocity2, dt;
  l = AckermannController::getL();
  // Initializing values of wheel radius and track width in metres
  wheelRadius = 0.3;
  trackWidth = 1;
  // Initializing time interval dt
  dt = 1.0;
  // Calculating Angular Velocity in radians/second
  angularVelocity = (reqTheta * M_PI) / (180 * dt);
  // Calculating turning radius
  turningRadius = reqV / angularVelocity;
  // Calculate ICRI for the inner wheel
  ICRI = sqrt(pow(l, 2) + turningRadius - pow((trackWidth / 2), 2));
  displacement1 = ICRI * angularVelocity;
  // Rotations per second of the inner wheel
  rps1 = displacement1 / wheelRadius;
  // Calculate drive velocity of inner wheel in meters/second
  driveVelocity1 = rps1 * M_PI * 2 * wheelRadius;
  // Calculate ICRO for the outer wheel
  ICRO = sqrt(pow(l, 2) + turningRadius + pow((trackWidth / 2), 2));
  displacement2 = ICRO * angularVelocity;
  // Rotations per second of the outer wheel
  rps2 = displacement2 / wheelRadius;
  // Calculate drive velocity of outer wheel in meters/second
  driveVelocity2 = rps2 * M_PI * 2 * wheelRadius;
  std::vector<double> driveVel { driveVelocity1, driveVelocity2 };
  return driveVel;
}
