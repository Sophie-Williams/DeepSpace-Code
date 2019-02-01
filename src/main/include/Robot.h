/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>

#include "toggle/toggle.cpp"

#include <array>
#include <vector>
#include <functional>
#include <frc/Spark.h>
#include <frc/Talon.h>
#include <frc/Joystick.h>
#include <frc/Solenoid.h>
#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/DoubleSolenoid.h>
#include <frc/drive/MecanumDrive.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <frc/smartdashboard/Smartdashboard.h>
#include <frc/smartdashboard/SendableChooser.h>

class Robot : public frc::TimedRobot {
public:
	//core robot functions
    void RobotInit() override;
    void RobotPeriodic() override;
    void AutonomousInit() override;
    void AutonomousPeriodic() override;
    void TeleopInit() override;
    void TeleopPeriodic() override;
    void TestPeriodic() override;

	//helper functions
    void toggleSolenoid(frc::DoubleSolenoid& solenoid);

	//limelight variables
    enum limelight_target_enum { //all possible targets we might want to align to
		FEEDER_HATCH_GROUND,
		FEEDER_BALL_GROUND_LEFT, FEEDER_BALL_UPPER_LEFT, FEEDER_HATCH_UPPER_LEFT,
		FEEDER_BALL_GROUND_RIGHT, FEEDER_BALL_UPPER_RIGHT, FEEDER_HATCH_UPPER_RIGHT,
		ROCKET_BALL_GROUND, ROCKET_BALL_UPPER, ROCKET_HATCH_GROUND, ROCKET_HATCH_UPPER,
		CARGO_BALL_GROUND, CARGO_BALL_UPPER, CARGO_HATCH_GROUND, CARGO_HATCH_UPPER,
	};

	//NOTE: All low goal hatches has tape opens downwards "\ /", but the left feeder station has them opening upwards "/ \"
    enum limelight_pattern_enum { GROUND, BALL, HATCHES, FEEDER_LEFT };

	//limelight functions
    void limelightUpdate(limelight_pattern_enum pattern); //updates the table data
    void limelightMove(limelight_target_enum target); //gets distance from
    bool limelightCheck(limelight_target_enum target); //checks to see if the current data is from the correct pipe
	//get what pattern is associated with a given target
    limelight_pattern_enum limelightConvert(limelight_target_enum target);

   private:
    //smartdashboard objects
    frc::SendableChooser<std::string> m_chooser;
    const std::string kAutoNameDefault = "Default";
    const std::string kAutoNameCustom = "My Auto";
    std::string m_autoSelected;

	//joysticks
    frc::XboxController controller_left { 0 };
    frc::XboxController controller_right { 1 };

    //drive train motors + drive train
    frc::Talon drive_NW{0};
    frc::Talon drive_NE{1};
    frc::Talon drive_SE{2};
    frc::Talon drive_SW{3};
    frc::MecanumDrive drive_train{drive_NW, drive_NE, drive_SE, drive_SW};

	//other motors
    frc::Spark arm { 4 };

    //phenumatics
    frc::DoubleSolenoid phenumatic_grabber { 0, 1 }; //grabs hatches
    bool phenumatic_grabber_grabbing=false;

    frc::DoubleSolenoid phenumatic_climber { 2, 3 }; //grabs HAB zone
	bool phenumatic_climber_grabbing=false;

	frc::DoubleSolenoid phenumatic_arm { 4, 5 }; //lowers/raises the lower arm
	bool phenumatic_arm_grabbing=false;

    frc::DoubleSolenoid phenumatic_deployer { 6, 7 }; //deploys ramp
	bool phenumatic_deployer_grabbing=false;
    int phenumatic_deployer_min=5; //min amount to press "X" to deploy
    int phenumatic_deployer_safety=0; //makes sure that ramps dont deploy on accident

    //limelight objects
	std::shared_ptr<NetworkTable> limelight=nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    limelight_pattern_enum limelight_pipe; //makes sure the current pipeline is of the pattern we want
    double limelight_area=0;  //empty defines for current offsets
    double limelight_skew=0;
	double limelight_offset_horz=0;
	double limelight_offset_vert=0;
};