#include "MotorUnion.h"
#include <unistd.h>
#include <termio.h>
using namespace std;
vector<unsigned char> MotorUnion::allport = {0, 1, 2, 3, 4, 5, 6};

/**
 * Motor constructor
 * 
 * @param IDArray - 
 * @param MotorModelArray - Array defined the type of motor using
 */
MotorUnion::MotorUnion(const vector<unsigned char> &IDArray,
					   const vector<string> &MotorModelArray)
	: _is_deleted_thread_BG(true),
	  waiting_frequency(10)
{
	for (unsigned char i = 0; i < IDArray.size(); i++)
	{
		if (MotorModelArray.at(i) == "Pro100" || MotorModelArray.at(i) == "Pro200" || MotorModelArray.at(i) == "Pro20")
			Motor_Union.push_back(new MotorPro(IDArray.at(i), MotorModelArray.at(i)));

		else if (MotorModelArray.at(i) == "Pro20+")
			Motor_Union.push_back(new MotorProPlus(IDArray.at(i), MotorModelArray.at(i)));

		else if (MotorModelArray.at(i) == "Xm540" || MotorModelArray.at(i) == "Xm430")
			Motor_Union.push_back(new MotorXm(IDArray.at(i), MotorModelArray.at(i)));

		else if (MotorModelArray.at(i) == "Mx28" || MotorModelArray.at(i) == "Mx106")
			Motor_Union.push_back(new MotorMx(IDArray.at(i), MotorModelArray.at(i)));
		else
			;
	}

	if (ConnectAllMotors(MotorUnion::allport))
		BGON();
}

MotorUnion::~MotorUnion()
{
	if (!_is_deleted_thread_BG)
	{
		_is_deleted_thread_BG = true;
		thread_BG->join();
		delete thread_BG;
	}

	deleteInVector(Motor_Union);
	deleteInVector(portHandler);
	deleteInVector(groupBulkRead);
	deleteInVector(groupBulkWrite);
}

template <class T>
void MotorUnion::deleteInVector(vector<T *> tmp_vector)
{
	while (!tmp_vector.empty())
	{
		delete tmp_vector.back();
		tmp_vector.pop_back();
	}
}

////////////////////////////////////////////////////////////////////////////////
///  All Motors   //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const bool MotorUnion::ConnectAllMotors(vector<unsigned char> &AllPortNumber)
{
	vector<unsigned char>::iterator it;
	for (it = AllPortNumber.begin(); it != AllPortNumber.end();)
	{
		// Set the port path
		string port_path = string("/dev/ttyUSB" + to_string(*it));
		// Initialize PortHandler & PacketHandler instance
		dynamixel::PortHandler *tmp_portHandler = dynamixel::PortHandler::getPortHandler(port_path.c_str());
		packetHandler = dynamixel::PacketHandler::getPacketHandler(2.0);
		dynamixel::GroupBulkRead *tmp_groupBulkRead = new dynamixel::GroupBulkRead(tmp_portHandler, packetHandler);
		dynamixel::GroupBulkWrite *tmp_groupBulkWrite = new dynamixel::GroupBulkWrite(tmp_portHandler, packetHandler);

		bool port_gate = false;
		for (int i = 0; i < Motor_Union.size(); i++)
		{
			if (Motor_Union.at(i)->GetMotorConnected() == false)
			{
				Motor_Union.at(i)->ConnectDynamixel(tmp_portHandler, packetHandler, tmp_groupBulkRead, tmp_groupBulkWrite);
				port_gate |= Motor_Union.at(i)->GetMotorConnected();
			}
		}
		if (port_gate == true)
		{
			it = AllPortNumber.erase(it);
			portHandler.push_back(tmp_portHandler);
			groupBulkRead.push_back(tmp_groupBulkRead);
			groupBulkWrite.push_back(tmp_groupBulkWrite);
		}
		else
		{
			++it;
			delete tmp_groupBulkWrite;
			delete tmp_groupBulkRead;
			delete tmp_portHandler;
		}
	}

	// Check every motor is connected
	bool connected = true;
	for (int i = 0; i < Motor_Union.size(); i++)
		connected &= Motor_Union.at(i)->GetMotorConnected();

	return connected;
}

const bool MotorUnion::CheckAllMotorsConnected() const
{
	bool connected = true;
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		Motor_Union.at(i)->ConnectDynamixel();
		connected &= Motor_Union.at(i)->GetMotorConnected();
	}
	return connected;
}

const bool MotorUnion::CheckAllMotorsArrival() const
{
	bool arrival = true;
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		arrival &= Motor_Union.at(i)->GetMotor_Arrival();
	}
	return arrival;
}

/**
 * Wait for i-th motor to arrive
 * 
 * @param i - ID of the motor
 */
void MotorUnion::WaitMotorArrival(int i) const
{
	while (!Motor_Union.at(i)->GetMotor_Arrival())
	{
		if (GetAllMotorsTorqueEnable() == false)
			break;
		this_thread::sleep_for(chrono::milliseconds(waiting_frequency));
	}
}

void MotorUnion::WaitAllMotorsArrival() const
{
	while (!CheckAllMotorsArrival())
	{
		if (GetAllMotorsTorqueEnable() == false)
			break;
		this_thread::sleep_for(chrono::milliseconds(waiting_frequency));
	}
}

void MotorUnion::WaitAllMotorsArrival(const int &total_waiting_time_ms) const
{
	for (int i = 0; i < total_waiting_time_ms / waiting_frequency; i++)
	{
		if (GetAllMotorsTorqueEnable() == false)
			break;
		this_thread::sleep_for(chrono::milliseconds(waiting_frequency));
	}
}

//------------------------------------------------------------------------------//
/*
	Get All Motors Data
*/
const bool MotorUnion::GetAllMotorsTorqueEnable() const
{
	bool tmp = true;
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		tmp &= GetMotor_TorqueEnable(i);
	}
	return tmp;
}

//------------------------------------------------------------------------------//
/*
	Set Motors Data
*/
void MotorUnion::SetAllMotorsOperatingMode(const unsigned char &mode) const
{
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		SetMotor_Operating_Mode(i, mode);
	}
}

void MotorUnion::SetAllMotorsAngle(const float &angle) const
{
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		SetMotor_Angle(i, angle);
	}
}

void MotorUnion::SetAllMotorsVelocity(const int &velocity) const
{
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		SetMotor_Velocity(i, velocity);
	}
}

void MotorUnion::SetAllMotorsAccel(const int &accel) const
{
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		SetMotor_Accel(i, accel);
	}
}

void MotorUnion::SetAllMotorsTorqueEnable(const bool &enable) const
{
	for (int i = 0; i < Motor_Union.size(); i++)
	{
		SetMotor_TorqueEnable(i, enable);
	}
}

////////////////////////////////////////////////////////////////////////////////
///   Motor   //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------//
/*
	Get Motor Data
*/
const unsigned char &MotorUnion::GetMotor_ID(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotorID();
}

const bool &MotorUnion::GetMotor_Connected(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotorConnected();
}

const float &MotorUnion::GetMotor_Scale2RPM(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_Scale2RPM();
}

const float &MotorUnion::GetMotor_Scale2RPMM(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_Scale2RPMM();
}

const short &MotorUnion::GetMotor_CenterScale(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_CenterScale();
}

const float &MotorUnion::GetMotor_Angle(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_Angle();
}

const int &MotorUnion::GetMotor_Velocity(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_Velocity();
}

const int &MotorUnion::GetMotor_Accel(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_Accel();
}

const bool &MotorUnion::GetMotor_TorqueEnable(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_TorqueEnable();
}

const float &MotorUnion::GetMotor_PresentAngle(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_PresentAngle();
}

const float &MotorUnion::GetMotor_PresentVelocity(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_PresentVelocity();
}

const float &MotorUnion::GetMotor_PresentTorque(const unsigned char &idx) const
{
	return Motor_Union.at(idx)->GetMotor_PresentTorque();
}

//------------------------------------------------------------------------------//
/*
	Set Motor Data
*/
void MotorUnion::SetMotor_Operating_Mode(const unsigned char &idx, char mode) const //can't set mode online
{
	// Motor_Union.at(idx)->SetMotor_TorqueEnable(false);
	// this_thread::sleep_for(chrono::milliseconds(50));
	Motor_Union.at(idx)->SetMotor_Operating_Mode(mode);
	// Motor_Union.at(idx)->WriteMode(mode);
	// this_thread::sleep_for(chrono::milliseconds(50));
	// Motor_Union.at(idx)->SetMotor_TorqueEnable(true);
}

void MotorUnion::SetMotor_CenterScale(const unsigned char &idx, const short &motor_center_scale) const
{
	Motor_Union.at(idx)->SetMotor_CenterScale(motor_center_scale);
}

void MotorUnion::SetMotor_Angle(const unsigned char &idx, const float &angle) const
{
	Motor_Union.at(idx)->SetMotor_Angle(angle);
}

void MotorUnion::SetMotor_Velocity(const unsigned char &idx, const int &velocity) const
{
	Motor_Union.at(idx)->SetMotor_Velocity(velocity);
}

void MotorUnion::SetMotor_Accel(const unsigned char &idx, const int &accel) const
{
	Motor_Union.at(idx)->SetMotor_Accel(accel);
}

void MotorUnion::SetMotor_TorqueEnable(const unsigned char &idx, const bool &enable) const
{
	Motor_Union.at(idx)->SetMotor_TorqueEnable(enable);
}

void MotorUnion::SetMotor_AngleandVelocity(const unsigned char &idx, const float &angle, const int &velocity) const
{
	Motor_Union.at(idx)->SetMotor_Velocity(velocity);
	Motor_Union.at(idx)->SetMotor_Angle(angle);
}

////////////////////////////////////////////////////////////////////////////////
///   Background   /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MotorUnion::BGON()
{
	_is_deleted_thread_BG = false;
	thread_BG = new thread(&MotorUnion::BGReadWrite, this);
}

void MotorUnion::BGReadWrite()
{
	while (!_is_deleted_thread_BG)
	{
		WriteData();
		ReadData();
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void MotorUnion::WriteData() const
{
	// Add parameters
	bool is_Write = false;
	for (int i = 0; i < Motor_Union.size(); i++)
		is_Write |= Motor_Union.at(i)->WriteData();

	// Write to motor
	if (is_Write)
	{
		for (int i = 0; i < groupBulkWrite.size(); i++)
		{
			int dxl_comm_result = groupBulkWrite.at(i)->txPacket();
			if (dxl_comm_result != COMM_SUCCESS)
				printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
			groupBulkWrite.at(i)->clearParam();
		}
	}
}

void MotorUnion::ReadData() const
{
	// Add parameters
	for (int i = 0; i < Motor_Union.size(); i++)
		Motor_Union.at(i)->AddParam();

	// Read Data
	for (int i = 0; i < groupBulkRead.size(); i++)
	{
		int dxl_comm_result = groupBulkRead.at(i)->txRxPacket();
		if (dxl_comm_result != COMM_SUCCESS)
			printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
	}

	// Record to Motor
	for (int i = 0; i < Motor_Union.size(); i++)
		Motor_Union.at(i)->ReadData();

	// clear parameters
	for (int i = 0; i < groupBulkRead.size(); i++)
		groupBulkRead.at(i)->clearParam();
}

const int &MotorUnion::Set_Velocity(int id, int mode, bool enable, int velocity)
{
	SetAllMotorsOperatingMode(1); //need to set mode on wizard 2.0 !!!!!!!!!!!!!!!!!!!!!!!!!!

	usleep(1000);

	SetMotor_TorqueEnable(id, enable);
	usleep(1000);

	SetMotor_Velocity(id, velocity);

	usleep(1000);

	return GetMotor_Velocity(id);
}

int MotorUnion::Leg_id(int id)
{

	const int arr[] = {1, 2, 4, 8}; //key in current motor id
	const int n = sizeof(arr) / sizeof(arr[0]);
	int i = 0;
	while (i < n)
	{
		if (arr[i] == id)
		{
			id = i;
			return id;
			break;
		}
		i++;
	}
}

const int MotorUnion::Sync_Drive(int driver1_id, int driver2_id, int driver3_id, int driver4_id, int velocity, int time)
{

	Set_Velocity(Leg_id(driver1_id), 1, 1, velocity);
	// Set_Velocity(Leg_id(driver2_id), 1, 1, -velocity);
	// Set_Velocity(Leg_id(driver3_id), 1, 1, velocity);
	// Set_Velocity(Leg_id(driver4_id), 1, 1, -velocity);
	sleep(time);
	Set_Velocity(Leg_id(driver1_id), 1, 1, 0);
	// Set_Velocity(Leg_id(driver2_id), 1, 1, 0);
	// Set_Velocity(Leg_id(driver3_id), 1, 1, 0);
	// Set_Velocity(Leg_id(driver4_id), 1, 1, 0);
	sleep(1);
	SetMotor_TorqueEnable(Leg_id(driver1_id), false);
	// SetMotor_TorqueEnable(Leg_id(driver2_id), false);
	// SetMotor_TorqueEnable(Leg_id(driver3_id), false);
	// SetMotor_TorqueEnable(Leg_id(driver4_id), false);
}

const int MotorUnion::Drive(int driver1_id, int velocity, int time)
{

	cout << "driver1 velocity is: " << Set_Velocity(Leg_id(driver1_id), 1, 1, velocity) << endl;
	sleep(time);
	cout << "driver1 velocity is: " << Set_Velocity(Leg_id(driver1_id), 1, 1, 0) << endl;
	sleep(1);
	SetMotor_TorqueEnable(Leg_id(driver1_id), false);
}

int MotorUnion::scanKeyboard()
{
	//  https://www.cxyzjd.com/article/Q_upup/104848283
	int in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(STDIN_FILENO, &stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(STDIN_FILENO, &stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

	in = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &stored_settings);
	return in;
}

const int MotorUnion::TurnRight(int Velocity, int Right_key, int time)
{

	cout << " Turn right : " << time << " seconds." << endl;
	Set_Velocity(Leg_id(1), 1, 1, Velocity);
	// Set_Velocity(Leg_id(2), 1, 1, Velocity);
	// Set_Velocity(Leg_id(4), 1, 1, Velocity);
	// Set_Velocity(Leg_id(8), 1, 1, Velocity);
	sleep(time);
	Set_Velocity(Leg_id(1), 1, 1, 0);
	// Set_Velocity(Leg_id(2), 1, 1, 0);
	// Set_Velocity(Leg_id(4), 1, 1, 0);
	// Set_Velocity(Leg_id(8), 1, 1, 0);
	sleep(1);
	SetMotor_TorqueEnable(Leg_id(1), false);
	// SetMotor_TorqueEnable(Leg_id(2), false);
	// SetMotor_TorqueEnable(Leg_id(4), false);
	// SetMotor_TorqueEnable(Leg_id(8), false);
}

const int MotorUnion::TurnLeft(int Velocity, int Right_key, int time)
{

	cout << " Turn left : " << time << " seconds." << endl;
	Set_Velocity(Leg_id(1), 1, 1, -Velocity);
	// Set_Velocity(Leg_id(2), 1, 1, -Velocity);
	// Set_Velocity(Leg_id(4), 1, 1, -Velocity);
	// Set_Velocity(Leg_id(8), 1, 1, -Velocity);
	sleep(time);
	Set_Velocity(Leg_id(1), 1, 1, 0);
	// Set_Velocity(Leg_id(2), 1, 1, 0);
	// Set_Velocity(Leg_id(4), 1, 1, 0);
	// Set_Velocity(Leg_id(8), 1, 1, 0);
	sleep(1);
	SetMotor_TorqueEnable(Leg_id(1), false);
	// SetMotor_TorqueEnable(Leg_id(2), false);
	// SetMotor_TorqueEnable(Leg_id(4), false);
	// SetMotor_TorqueEnable(Leg_id(8), false);
}

const int MotorUnion::GoStraight(int Velocity, int Straight_key, int time)
{
	cout << " Go straight : " << time << " seconds." << endl;
	Sync_Drive(1, 2, 4, 8, Velocity, time);
	// Sync_Drive(0, 2, Velocity, time);
}

const int MotorUnion::GoBack(int Velocity, int Back_key, int time)
{
	cout << " Go back : " << time << " seconds." << endl;
	Sync_Drive(1, 2, 4, 8, -Velocity, time);
}

int MotorUnion::KeepMoving(int Velocity, int KeepMove_key)
{
	int Stop = 0;
	cout << " Keep moving. " << endl;
	while (KeepMove_key == 107)
	{
		Set_Velocity(Leg_id(1), 1, 1, Velocity);
		// Set_Velocity(Leg_id(2), 1, 1, -Velocity);
		// Set_Velocity(Leg_id(4), 1, 1, Velocity);
		// Set_Velocity(Leg_id(8), 1, 1, -Velocity);
		cout << " Press any key except 'k' to stop. " << endl;
		KeepMove_key = scanKeyboard();

		Set_Velocity(Leg_id(1), 1, 1, 0);
		// Set_Velocity(Leg_id(2), 1, 1, 0);
		// Set_Velocity(Leg_id(4), 1, 1, 0);
		// Set_Velocity(Leg_id(8), 1, 1, 0);
		sleep(1);
		SetMotor_TorqueEnable(Leg_id(1), false);
		// SetMotor_TorqueEnable(Leg_id(2), false);
		// SetMotor_TorqueEnable(Leg_id(4), false);
		// SetMotor_TorqueEnable(Leg_id(8), false);
	}
	return Stop;
}
