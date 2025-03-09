#include "state.hpp"

State* State::init() {

	auto new_state = new State {};
	State::instance = new_state;

	return new_state;

};

State* State::get_instance() {

	return State::instance;

}