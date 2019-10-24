#include "Platoon.h"

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

            if (head == NULL)
            {
                head = car;
                tail = car;
            }
            else
            {
            	this->insert(car);
            }
		}
	}
}

//Removing a car from a platoon
void Platoon::remove(Car* c) {
	//IF only 1 car is in the platoon
	if (c == head && c == tail) {
		head = NULL;
		tail = NULL;
		c->set_next(NULL);
		c->set_prev(NULL);
	}
	//At the end of a platoon
	else if (c == tail) {
		Car* previousCar= c->get_prev();
		previousCar->set_next(NULL);
		tail = previousCar;
		c->set_next(NULL);
		c->set_prev(NULL);
	}
	//At the start of the platoon
	else if (c == head) {
		Car* nextCar = c->get_next();
		nextCar->set_prev(NULL);
		head = nextCar;
		c->set_next(NULL);
		c->set_prev(NULL);
	}
	//Removing from anywhere in the platoon
	else {
		Car* previousCar = c->get_prev();
		Car* nextCar = c->get_next();
		//remaking the connections in the list
		previousCar->set_next(nextCar);
		nextCar->set_prev(previousCar);

		c->set_next(NULL);
		c->set_prev(NULL);
	}
}
//Function which places a car at the tail of a linked list, a platoon
void Platoon::append(Car* c){
	//Empty platoon
	if (head == NULL && tail == NULL) {
		tail = c;
		head = c;
		c->set_next(NULL);
		c->set_prev(NULL);
	}
	else {
		//appending by getting the tail, and setting it to point next to c and c to point back at tail, before setting tail to c
		Car* last = Platoon::get_tail();
		last->set_next(c);
		c->set_prev(last);
		c->set_next(NULL);
		tail = c;
	}
}

//Function which places a car at the head of a linked list, a platoon
void Platoon::prepend(Car* c){
	//Empty platoon
	if (head == NULL && tail == NULL) {
			tail = c;
			head = c;
			c->set_next(NULL);
			c->set_prev(NULL);
	}
	else {
		//Prepending by setting the head to point to c, and c to point to head, then setting head to point to c
		Car* first = Platoon::get_head();
		first->set_prev(c); //setting old head to point back to c
		c->set_next(first);
		c->set_prev(NULL);
		head = c; //NEW hEAD
	}
}

void Platoon::insert(Car* c){
		Car* p = head;
		//If the platoon is empty
		if (head == NULL) {
			append(c);
		}
		//If c is smaller than head, it is prepended to the platoon
		else if (p == head && (p->get_position() > c->get_position()) ) {
			prepend(c);
		}
		//If c is larger than tail, it is appended to the platoon
		else if (p == tail && (p->get_position() < c->get_position()) ) {
			append(c);
		}
		else {
			//While loop which breaks either when p is null, or when p is found to be behind c and the car in front of p is in front of c
			while( p != NULL && p->get_next()->get_position() < c->get_position() && p->get_position() > c->get_position() ) {
				p = p->get_next(); //iterating
			}
				//inserting into position
				c->set_next(p->get_next());
				c->set_prev(p);
				p->get_next()->set_prev(c);
				p->set_next(c);
		}
}
//Function which checks if a space is free or not, return true if a space is taken, and false when a space is free
bool Platoon::spaceCheck(int carpos) {
	Car* iterate = tail;
	while (iterate != NULL) {
		//comparing the two positions to see if they are the same spot
		if (iterate->get_position() == carpos) {
			return true;
		}
			iterate = iterate->get_prev(); //iterating
	}
	return false;
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

