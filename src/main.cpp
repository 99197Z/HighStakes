#include "main.h"
#include "display/lv_core/lv_obj.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "autoSelect/selection.h"

#define UseLVGL


#define FL 1
#define FR 11
#define AL 2
#define AR 12

#define Arm 3
#define arm_move 45

int auton = 0;

lv_obj_t * myButton;
lv_obj_t * myButtonLabel;
lv_obj_t * myLabel;

lv_style_t myButtonStyleREL; //relesed style
lv_style_t myButtonStylePR; //pressed style

static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn); //id usefull when there are multiple buttons

    if(id == 0)
    {
        char buffer[100];
		sprintf(buffer, "button was clicked %i milliseconds from start", pros::millis());
		lv_label_set_text(myLabel, buffer);
		selector::init();
    }

    return LV_RES_OK;
}

void lvglInit() {
	lv_style_copy(&myButtonStyleREL, &lv_style_plain);
    myButtonStyleREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
    myButtonStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
    myButtonStyleREL.body.radius = 0;
    myButtonStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

    lv_style_copy(&myButtonStylePR, &lv_style_plain);
    myButtonStylePR.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    myButtonStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
    myButtonStylePR.body.radius = 0;
    myButtonStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);

    myButton = lv_btn_create(lv_scr_act(), NULL); //create button, lv_scr_act() is deafult screen object
    lv_obj_set_free_num(myButton, 0); //set button is to 0
    lv_btn_set_action(myButton, LV_BTN_ACTION_CLICK, btn_click_action); //set function to be called on button click
    lv_btn_set_style(myButton, LV_BTN_STYLE_REL, &myButtonStyleREL); //set the relesed style
    lv_btn_set_style(myButton, LV_BTN_STYLE_PR, &myButtonStylePR); //set the pressed style
    lv_obj_set_size(myButton, 200, 50); //set the button size
    lv_obj_align(myButton, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); //set the position to top mid

    myButtonLabel = lv_label_create(myButton, NULL); //create label and puts it inside of the button
    lv_label_set_text(myButtonLabel, "Click the Button"); //sets label text

    myLabel = lv_label_create(lv_scr_act(), NULL); //create label and puts it on the screen
    lv_label_set_text(myLabel, "Button has not been clicked yet"); //sets label text
    lv_obj_align(myLabel, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0); //set the position to center
}

/*
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
  autonomous();
}



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  //pros::lcd::initialize();
  //pros::lcd::set_text(0, "speed?");
//
  //pros::lcd::register_btn1_cb(on_center_button);
  #ifdef UseLVGL
  selector::init();
  #endif
  // Sanity 0
  pros::Motor FL_init(FL, pros::E_MOTOR_GEAR_GREEN, true,pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor FR_init(FR, pros::E_MOTOR_GEAR_GREEN, true,pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor AL_init(AL, pros::E_MOTOR_GEAR_GREEN, true,pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor AR_init(AR, pros::E_MOTOR_GEAR_GREEN, true,pros::E_MOTOR_ENCODER_DEGREES);

  pros::Motor ARM_init(Arm, pros::E_MOTOR_GEAR_GREEN, true,pros::E_MOTOR_ENCODER_DEGREES);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	pros::Motor fl_mtr(FL);
  	pros::Motor fr_mtr(FR);
  	pros::Motor al_mtr(AL);
  	pros::Motor ar_mtr(AR);
  	pros::MotorGroup left_mtr({fl_mtr, al_mtr});
  	pros::MotorGroup right_mtr({fr_mtr, ar_mtr});

  	pros::Motor arm_mtr(Arm);
	arm_mtr.move_absolute(90, 100);
	pros::delay(1000);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  //pros::lcd::initialize();
  pros::Controller master(pros::E_CONTROLLER_MASTER);

  pros::Motor fl_mtr(FL);
  pros::Motor fr_mtr(FR);
  pros::Motor al_mtr(AL);
  pros::Motor ar_mtr(AR);
  pros::MotorGroup left_mtr({fl_mtr, al_mtr});
  pros::MotorGroup right_mtr({fr_mtr, ar_mtr});

  pros::Motor arm_mtr(Arm);

  while (true) {
    /*pros::lcd::print(1, "%d %d %d %d",
                     (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                     (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                     (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0,
					 selector::auton);*/
    int left = master.get_analog(ANALOG_LEFT_Y);
    int right = master.get_analog(ANALOG_RIGHT_Y);

    left_mtr.move(left);
    right_mtr.move(right);

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      arm_mtr.move(arm_move);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      arm_mtr.move(-arm_move);
	} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
		autonomous();
		pros::delay(200);
    } else {
      arm_mtr.brake();
    }
    pros::delay(20);
  }
}
