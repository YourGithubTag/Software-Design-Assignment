#include "TrafficModel.h"

TrafficModel::TrafficModel() { }

TrafficModel::~TrafficModel(){
	//looping through all platoons such that all cars can be deleted from the heap
	for (unsigned int currentPlatoon = 0; currentPlatoon < platoons.size(); currentPlatoon++){
		Car* iterate = platoons[currentPlatoon].get_tail();
		Car* temp;
		while (iterate != NULL){
			temp = iterate->get_prev();
			delete iterate; //deleting car
			iterate = temp; //iterating
		}
	}
}

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

 /* The function that updates the vehicle positions and states.
 */

void TrafficModel::update()
{
	int turnSignal;
	int checked;
	int nextpos;
	//for loop which loops through all platoons
	for (unsigned int currentPlatoon = 0; currentPlatoon < platoons.size(); currentPlatoon++){
		Car* iterate = platoons[currentPlatoon].get_tail(); //car we are checking, starting at tail and working our way back
		Car* temp; // variable which gets the car behind the one we are checking, such that it will be the next car checked.
		int nextpos;
			while (iterate != NULL){
				temp = iterate->get_prev(); //obtaining car behind iterate
				turnSignal = get_lane_change_command(iterate->get_id()); //turn signal command
				nextpos = iterate->get_position()+1; //value of the position ahead
				checked = Validmove(iterate, currentPlatoon, turnSignal); //getting which command should be executed
					//switch statement based on which command should be executed
					switch(checked) {
					case 0:
						//moving forward
						iterate->set_position(nextpos);
						break;

					case 1:
						//making a left turn
						platoons[currentPlatoon].remove(iterate);
						platoons[currentPlatoon - 1].insert(iterate);
						break;
					case 2:
						//making a right turn and setting flag so a car cannot be moved twice
						iterate->set_hasTurned(true);
						platoons[currentPlatoon].remove(iterate);
						platoons[currentPlatoon + 1].insert(iterate);
						break;
					case 3:
						//resetting the turning flag, once a car has been denied a move
						iterate->set_hasTurned(false);
						break;
			}
			iterate = temp; //moving backwards in the platoon
			}
	}
}
//function which takes the car, the current platoon and the turn signal the car will execute, and checks if it is a valid move
int TrafficModel::Validmove(Car* iterate, unsigned int currentPlatoon, int turnSignal) {
	Car* checking = iterate;
	int platoon = currentPlatoon;
	//checking if a car has already made a move
	if (!iterate->get_hasTurned()){
		if (turnSignal == 0){
			//checking if a car could move forward
			if (checking->get_next() == NULL || checking->get_next()->get_position() != checking->get_position() + 1) {
				return 0;
			}
			else {
				return 3; //denied a move
			}
		}

		else if (turnSignal == 1){
			//checking if a car could move to the left, by checking if it is not going out of bounds and if the space is occupied or not
			if (currentPlatoon != 0 && !(platoons[currentPlatoon - 1].spaceCheck(iterate->get_position()) )) {
				return 1; //the left turn can be made
			}
			else {
				return Validmove(checking, platoon,0); //car cannot make a left turn, recursion used to see if it can move forward
			}
		}

		else if (turnSignal == 2){
			//checking if a car can move right, by checking if it is trying to make an out of bounds move and if the space is occupied or not
			if ((currentPlatoon != platoons.size() + 1 ) && !(platoons[currentPlatoon + 1].spaceCheck(iterate->get_position()) )) {
				return 2; // the turn can be made
			}
			else {
				return Validmove(checking, platoon,0); // recursion used to see if it can move forward
			}
		}
	}
	//car has been denied a move, due to already moving before
	return 3;
}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}
