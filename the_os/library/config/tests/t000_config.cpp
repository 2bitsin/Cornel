#include <config/config.hpp>
#include "expect.hpp"

int main(int argc, char **argv)
{ 
	config::store s;
	
	s.set("Hello", "World");
  return 0;
}