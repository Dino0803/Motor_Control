#include <MotorUnion.h>
#include <unistd.h> /* UNIX standard function definitions */
#include <termio.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    MotorUnion motor_driver({1}, {"Mx106"}); 
    motor_driver.SetMotor_Operating_Mode(0, 1);
    motor_driver.SetMotor_TorqueEnable(0, true);
    while (1)
    {
        cout << "-------------Type  SpeedMode( 1, 2, 3 ) : Direction( w(Go), s(Back), a(Left), d(Right) ) : Time( Interger )----------------"
             << endl;
        //motor_driver2.Sync_Drive(4, 8, 100, 2);
        int speed_mode = motor_driver.scanKeyboard();
        int speed;
        int key;
        int time;

        if (speed_mode == 10)
        {
            speed_mode = motor_driver.scanKeyboard();
        }

        switch (speed_mode)
        {
        case 49:
            speed = 77;
            key = motor_driver.scanKeyboard();
            break;
        case 50:
            speed = 154;
            key = motor_driver.scanKeyboard();
            break;
        case 51:
            speed = 230;
            key = motor_driver.scanKeyboard();
            break;
        default:
            speed = 0;
            cout << endl
                 << "No this mode." << endl;
            break;
        }

        if (speed == 0)
        {
            key = 0;
        }
        else if (key == 107)
        {
            key = motor_driver.KeepMoving(speed, key);
        }

        switch (key)
        {
        case 0:
            break;
        case 100:
            cin >> time;
            motor_driver.TurnRight(speed, key, time);
            break;
        case 97:
            cin >> time;
            motor_driver.TurnLeft(speed, key, time);
            break;
        case 119:
            cin >> time;
            motor_driver.SetMotor_Velocity(0, 100);
            break;
        case 115:
            cin >> time;
            motor_driver.GoBack(speed, key, time);
            break;
        default:
            cout << endl
                 << "No this direction." << endl;
            break;
        }
    }

    // motor_driver.Sync_Drive(0, 6, 210, 2);

    // motor_driver.Drive(0, 210, 2);
}

// int *ControlButton()
// {
//     int *array = new int[2];
//     array[0] = scanKeyboard();
//     array[1] = scanKeyboard();
//     //printf("%d\n",array);
//     return array;
// }

//////////////////code using MotorUnion.h/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// MotorUnion motor_driver({0,6},{"Mx28","Mx28"});
//  motor_driver.SetAllMotorsOperatingMode(1);         //need to set mode on wizard 2.0 !!!!!!!!!!!!!!!!!!!!!!!!!!
//  usleep(1000);

// motor_driver.SetMotor_TorqueEnable(0, true) ;
//  usleep(1000);
// motor_driver.SetMotor_TorqueEnable(1, true) ;
//  usleep(1000);
// motor_driver.SetMotor_Velocity(0,  200);
// usleep(5000);
// motor_driver.SetMotor_Velocity(1,  200);
// sleep(2);
//  motor_driver.SetMotor_Velocity(0, 0);
//  usleep(5000);
// motor_driver.SetMotor_Velocity(1, 0);
// sleep(3);
// motor_driver.SetMotor_TorqueEnable(0,false ) ;
// motor_driver.SetMotor_TorqueEnable(1,false ) ;
