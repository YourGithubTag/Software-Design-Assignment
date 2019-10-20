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
            { //TODO use the insert function which inserts based off position
                tail->set_next(car);
                car->set_prev(tail);
                tail = car;
                car->set_next(NULL);
            }
		}
	}
}

//TODO comments
void Platoon::remove(Car* c) {
	Car* previousCar = c->get_prev();
	Car* nextCar = c->get_next();
	if (c == head && c == tail) {
		head = NULL;
		tail = NULL;
		std::cout << "remove successful" << std::endl;
	}
	else if (c == tail) {
		previousCar->set_next(NULL);
		tail = previousCar;
		c->set_next(NULL);
		c->set_prev(NULL);
		std::cout << "remove successful" << std::endl;
	}
	else if (c == head) {
		nextCar->set_prev(NULL);
		head = nextCar;
		c->set_next(NULL);
		c->set_prev(NULL);
		std::cout << "remove successful" << std::endl;
	}
	else {
		previousCar->set_next(nextCar);
		nextCar->set_prev(previousCar);

		c->set_next(NULL);
		c->set_prev(NULL);
		std::cout << "remove successful" << std::endl;
	}
}
void Platoon::append(Car* c){
	if (head == NULL && tail == NULL) {
		tail = c;
		head=c;
	}

	Car* last = Platoon::get_tail();
	last->set_next(c);
	c->set_prev(last);
	c->set_next(NULL);
	tail = c;

}
void Platoon::prepend(Car* c){
	Car* first = Platoon::get_head();
		first->set_prev(c);
		c->set_next(first);
		c->set_prev(NULL);
		head = c;
}

void Platoon::insert(Car* c){
	bool inserted = false;
	Car* p = head;
	while(!inserted) {  //CHECK FOR EQUALITY MAYBE
		std::cout << "checking if can be inserted" << std::endl;
		if (head == NULL && tail == NULL) {
			append(c);
			std::cout << "inserted into empty" << std::endl;
			inserted = true;
		}
		else if (p == tail  && p->get_position() < c->get_position()) {
			append(c);
			std::cout << "inserted into tail" << std::endl;
			inserted = true;
		}

		else if (p == head && p->get_position() > c->get_position() ) {
			prepend(c);
			std::cout << "inserted into head" << std::endl;
			inserted = true;
		}
		else if ( (p->get_next()->get_position() > c->get_position() ) && (p->get_position() < c->get_position()) ) {
				c->set_next(p->get_next());
				c->set_prev(p);

				p->get_next()->set_prev(c);
				p->set_next(c);
				std::cout << "insert successful" << std::endl;
				inserted = true;

		} else {
				std::cout << "NO" << std::endl;
				p = p->get_next();
		}

	}
	std::cout << "leaving Insert" << std::endl;
}
//TODO sort all platoons then make algorithmns assume this
void Platoon::sort() {
	/*Car* c = head;
	Car* d = head;
	bool swapped = false;
	   while(c != NULL){
		   while(d->get_next() != NULL){
				if (d->get_position() < d->get_next()->get_position() ){
					swap(d,d->get_next());
				}
	    }

}*/
}


Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

