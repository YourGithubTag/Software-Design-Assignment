#include "TrafficModel.h"

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

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

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
	// TODO: complete this function
	for (int i = 0; i < platoons.size(); i++){
		Car* iterate = platoons[i].get_tail();
		while (iterate != NULL){

			/*for (int j = 0; j < sizeof(commands); j++ ) { //TODO change structure of this loop

				//string com = commands[j];
				//com = SimulationUI::split(com);

				if (com[2] == iterate->get_id() ) { // check how to get car ID from Command
					if (Validcheck() == 1){
						platoons[i].remove(iterate);
						platoons[i-1].insert(iterate); //TODO tree up  insert
					}
					if (Validcheck() == 2){
						platoons[i].remove(iterate);
					    platoons[i+1].insert(iterate); //TODO  tree  down insert
					}
				}
				else {

					}
			}*/

			int nextpos = iterate->get_position() +1 ;
			iterate->set_position(nextpos);
			//moving backwards
			iterate = iterate->get_prev();
		}
	}
}

int TrafficModel::Validcheck() {

//TODO if statement to check if turning left into out of bounds

//TODO if statement to check if turning right into out of bounds

//TODO if statement to check the above or below lane and if there is a position is free

	//TODO if below
	//TODO if above
	return 0;

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
