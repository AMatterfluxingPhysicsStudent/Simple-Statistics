#include "llist.h"
#include "randomdata.h"

#include <iostream>
#include <vector>
#include <set>


int main(int argc, char* argv[])
{
	//Random Vector Generation
		Random_data<int> test;
		std::vector<int> v1 = test.get_random_vector(120, 1, 100); 
		std::cout << "Displaying all v1" << std::endl;
		std::cout << "{ ";
		for (int v : v1)
			std::cout << v << "  " ;
		std::cout << " }" <<std::endl;

    LList<int> i_linked_list;
    for(int v: v1)
        i_linked_list.append(v);
	std::cout << "Mode is " <<  i_linked_list.get_mode()[0] << " " << std::endl; //the edit: there wasn't a purpose in range-based loop outputting the mode several times
    std::cout << "Median is " << i_linked_list.get_median() << std::endl;
    std::cout << "Mean is " << i_linked_list.get_mean() << std::endl;
    std::cout << "SD is " << i_linked_list.get_SD() << std::endl;
    std::cout << "Min is " << i_linked_list.get_min() << std::endl;
    std::cout << "Max is " << i_linked_list.get_max() << std::endl;
	
	std::cout << "All is good" << "\n";
	return 0;
}
