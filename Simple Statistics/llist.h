#ifndef LLIST_H
#define LLIST_H

// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

#include "list.h"
#include "link.h"
#include <iostream>
#include <vector>
#include <set>
#include <iterator>

// Linked list implementation
template <typename E> class LList : public List<E> {
private:
	Link<E>* head; // Pointer to list header
	Link<E>* tail; // Pointer to last element
	Link<E>* curr; // Access to current element

	int cnt; // Size of list
	int sum; // sum of elements
	double pre_SD; // sum of (x - aveX)*(x - aveX)
	std::set<E> unique; //unique elements
	std::vector<E> total;

	void init() { // Initialization helper method
		curr = tail = head = new Link<E>;
		cnt = 0;
		sum = 0;
	}

	void removeall() { // Return link nodes to free store
		while (head != NULL) {
			curr = head;
			head = head->next;
			delete curr;
		}
		unique.clear();
		total.clear();
	}

	struct el_num {
		std::vector<E> element;
		int count = 0;
	};
	el_num count_el;

	/********************************
	 * here is some problem
	 */
	void max_count(E target) {
		for (E s : unique) {
			if (search(s).size() == count_el.count) {
				count_el.element.push_back(s);
			}
			else if (search(s).size() > count_el.count) {
				count_el.element.clear();
				count_el.element.push_back(s);
				count_el.count = search(s).size();
			}
		}
	}
	void total_sort() {
		sort(total.begin(), total.end());
	}

	void pre_SD_sum() {
		pre_SD = 0;
		for (E v : total) {
			auto tmp = v - this->get_mean();
			pre_SD += tmp * tmp;
		}
	}

public:
	LList(int size = 10) {
		init(); // Constructor replace defaultSize (now 10) with the default you want otherwise leave empty
	}
	template<class container>
	LList(container in) {
		init();
		feed(in);
	}
	~LList() { // Be very careful when you copy from pdf the tilde will not get copied properly so be sure to replace with actual
		removeall(); // Destructor
	}

	//1.append: append new data from the end
	void append(const E& it) { // Append "it" to list
		tail = tail->next = new Link<E>(it, NULL);
		cnt++;
		sum += it;
		total.push_back(it);
		unique.insert(it);
		total_sort();
		max_count(it);
		pre_SD_sum();
	}

	//2, Remove and return current element
	E remove() {
		//Assert(curr->next != NULL, "No element");
		E it = curr->next->element; // Remember value
		Link<E>* ltemp = curr->next; // Remember link node
		if (tail == curr->next) tail = curr; // Reset tail
		curr->next = curr->next->next; // Remove from list
		delete ltemp; // Reclaim space
		cnt--; // Decrements the count
		sum -= it;

		total.erase(total.begin() + search(it)[0] - 1);
		if (this->search(it).size() == 0)
			unique.erase(it);
		max_count(it);
		pre_SD_sum();
		return it;
	}

	//3. empty: delete all data
	void empty() {
		removeall();
		init(); // Clear list
	}

	// 4. feed: insert multiple data in one container
	template<class container>
	feed(container in) {
		for (auto it = std::begin(in); it != std::end(in); it++) {
			append(*it);
		}
	}

	// 5. search (fastest possible among the choices what we have  discussed in the class):
	// must return the location of the first occurrence and repetition.
	// For example, if the first occurrence of data value 20 is 3  and there are 10 of 20s,
	// then you must return 3 and 10 as the search result of 20
	std::vector<int> search(int target) {
		std::vector<int> result;

		this->moveToStart();
		for (int i = 0; i < this->cnt; i++) {
			if (this->getValue() == target) {
				result.push_back(i);
				this->next();
			}
		}
		return result;
	}
	// 6. array index operator, []: access each unique element through an index:
	// overload the array index operator ([]) so that the ith data element in an instance of your data object,
	// db can be accessed like db[i]
	std::set<int> operator[](int pos) {
		auto itr = unique.begin();
		for (int i = 0; i < pos; ++i)
			++itr;
		return itr;
	}

	//7. length_unique: number of unique elements in your data object
	int length_unique() const {
		return unique.size();
	}
	// 8, length_total: total number of elements in your data object
	int length_total() const {
		return cnt;
	}
	//9. unique: return all the unique elements as an std::set
	std::set<E> unique_set() const {
		return unique;
	}

	/*******************************
	 * 10. getters (all must be Θ(1)):
	 */
	 //10.1 get_mode: returns the mode value of the current entire data set
	std::vector<E> get_mode() {
		return count_el.element;
	}
	// 10.2 get_median: returns the median value of the current entire data set
	double get_median() {
		if (cnt % 2 == 0)
			return (total[cnt / 2] + total[cnt / 2 - 1]) / 2.0;
		return total[cnt / 2];
	}
	// 10.3 get_ mean: returns the mean value of the current entire data set
	double get_mean() {
		return (double)(sum) / cnt;
	}
	// 10.4 get_SD: returns the standard deviation  of the current entire data set
	double get_SD() {
		return sqrt((double)pre_SD / cnt);
	}
	// 10.5 get_min: returns the minimum data item in the current entire data set
	E get_min() {
		return total.front();
	}
	// 10.6 get_max: returns the maximum data item in the current entire data set
	E get_max() {
		return total.back();
	}


	// Insert "it" at current position (used in append)
	void insert(const E& it) {
		std::cout << "Its in" << std::endl;

		curr->next = new Link<E>(it, curr->next);
		if (tail == curr) tail = curr->next; // New tail
		cnt++;
	}

	// Place curr at list start (used in search)
	void moveToStart() {
		curr = head;
	}

	// Place curr at list end (can delete)
	void moveToEnd() {
		curr = tail;
	}
	// Move curr one step left; no change if already at front (can delete)
	void prev() {
		if (curr == head) return; // No previous element
		Link<E>* temp = head;
		// March down list until we find the previous element
		while (temp->next != curr) temp = temp->next;
		curr = temp;
	}
	// Move curr one step right; no change if already at end (cannot delete)
	void next() {
		if (curr != tail) curr = curr->next;
	}
	// Return the position of the current element (can delete)
	int currPos() const {
		Link<E>* temp = head;
		int i;
		for (i = 0; curr != temp; i++)
			temp = temp->next;
		return i;
	}
	// Move down list to "pos" position (can delete)
	void moveToPos(int pos) {
		//Assert ((pos>=0)&&(pos<=cnt), "Position out of range");
		curr = head;
		for (int i = 0; i < pos; i++) curr = curr->next;
	}
	const E& getValue() const { // Return current element (used int search)
	  //Assert(curr->next != NULL, "No value");
		return curr->next->element;
	}
	//(can delete)
	void reverse() {
		LList<E> result(10);
		E it;
		for (int i = 0; i < 8; i++) {
			curr = head;

			//while (curr != NULL) {
			it = remove();
			result.insert(it);
			//cout << "Its in" << endl;

		}
		curr = head;
		this->head = result.head;
	}
	bool isAtEnd() { return curr == tail; }

};

#endif // LLIST_H



/*
#ifndef LLIST_H
#define LLIST_H

// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.


#include "list.h"
#include "link.h"
#include <iostream>
#include "utilities.h"

#include <algorithm> //used for std::find to find if something is already in a vector

#include <vector>
#include <set>
#include <math.h> 


template <typename T>
struct pair {
	T firstindexpos; //pair.first //if need for value, subsititue value for firstindexpos in another struct pair
	T occurrences;//pair.second
};

// Linked list implementation
template <typename E> 
class LList: public List<E> {
private:
  Link<E>* head; // Pointer to list header
  Link<E>* tail; // Pointer to last element
  Link<E>* curr; // Access to current element

  int cnt; // Size of list
  int sum; // sum of elements

  double pre_SD; // sum of (x - aveX)*(x - aveX) //Ivan: so this is the numerator of the variance

  std::set<E> unique; 
  std::vector<E> total;  

  void init() { //note has issue of empty head
    curr = tail = head = new Link<E>;//This head exists, but empty
    cnt = 0;
    sum = 0;
  }

  void removeall() { // Return link nodes to free store
    while(head != NULL) {
        curr = head;
        head = head->next;
        delete curr;
      }
    unique.clear();  
    total.clear();  
  }


 
  struct el_num{ //this is basically a pair
      std::vector<E> modes;
      int count = 0;
  };
  el_num count_el;

  /********************************
   * here is some problem
   */
  void calc_mode(){
      for(E s: unique){
          if(search(s).occurrences == count_el.count) {
			  if (std::find(count_el.modes.begin(), count_el.modes.end(), s) != count_el.modes.end()) {} else {//if statement checking if s is already in el_num vector 
				  count_el.modes.push_back(s);
			  }
          }
		  else if(search(s).occurrences > count_el.count){
              count_el.modes.clear();
              count_el.modes.push_back(s);
              count_el.count = search(s).occurrences;
          }
      }
  }
  void total_sort(){
      sort(total.begin(), total.end());
  }

  void pre_SD_sum(){  
      pre_SD = 0;
      for(E v: total) {
          pre_SD += pow(v - this->get_mean(),2.0);
      }
  }

public:
  LList(int size = 10) {
    init(); // Constructor replace defaultSize (now 10) with the default you want otherwise leave empty
  }
  ~LList() { // Be very careful when you copy from pdf the tilde will not get copied properly so be sure to replace with actual
    removeall(); // Destructor
  }

  void clear() { //Ivan: imported this function from original llist.h to override the virtual clear() in abstract class Link<E> so Visual Studios would not refuse to compile LList because it is a virtual class
	  removeall();
	  init();
  }
  //1.append: append new data from the end
  void append(const E& it) { // Append "it" to list
      /*current =*/         tail = tail->next = new Link<E>(it, NULL);             //Ivan: might as well as add current too?
      cnt++;
      sum += it;
      total.push_back(it); //Ivan: wait, why does total exist at the same time as the linked list? Also, vector goes out of bounds
      unique.insert(it);
      total_sort();
      calc_mode();
      pre_SD_sum();
  }

  // Insert "it" at current position (used in append) //Ivan: insert() is unchanged from source
  void insert(const E& it) {
	  std::cout << "Its in" << std::endl;

	  curr->next = new Link<E>(it, curr->next);
	  if (tail == curr) tail = curr->next; // New tail
	  cnt++;
  }

  //2.	removen (from any place): remove n number of a given data element
  E removen() {
      Assert(curr->next != NULL, "No element"); 
	  
	  Link<E>* currentcurr = curr;//Without these few lines, remove() removes next curr, not the curr you're on
	  this->curr = this->head;
	  while (curr->next != currentcurr)
		  curr = curr->next;


      E it = curr->next->element; // Remember value
      Link<E>* ltemp = curr->next; // Remember link node
      if (tail == curr->next) tail = curr; // Moves tail away from about-to-be-deleted node
      curr->next = curr->next->next; // Remove from list by linking curr->next to the node after the about-to-be deleted node
      delete ltemp; // Reclaim space
      cnt--; // Decrements the count
      sum -= it;
	  //for(E v : total)
		//std::cout << v << std::endl; 

	  
      if(this->search(it).occurrences == 0)  //if the singular removal of it above results in removing the last instance of that value, then remove from unique
          unique.erase(it);
      calc_mode();
      pre_SD_sum();

	  delete currentcurr;

      return it;
  }

  //3. empty: delete all data
  void empty() {
    removeall();
    init(); // Clear list
  }

  // 5. search (fastest possible among the choices what we have  discussed in the class):
  // must return the location of the first occurrence and repetition.
  // For example, if the first occurrence of data value 20 is 3  and there are 10 of 20s,
  // then you must return 3 and 10 as the search result of 20


  pair<E> search(E target){
      pair<E> result; //custom struct
	  result.occurrences = 0;

      this->moveToStart(); //of the linked list
      for(int i=0; i < this->cnt; i++){
          if(this->getValue() == target && result.occurrences == 0){
			  result.firstindexpos = i;
			  result.occurrences++;
		  }
		  else if (this->getValue() == target) {
			  result.occurrences++;
		  }
		  this->next();//this line needs to be outside the loop, otherwise we don't move through the linked list at all
      }

      return result; 
	   
  }
  // 6. array index operator, []: access each unique element through an index:
  // overload the array index operator ([]) so that the ith data element in an instance of your data object,
  // db can be accessed like db[i]
  std::set<int> operator[](int pos){
      auto itr = unique.begin();
      for(int i = 0; i < pos; ++i)
          ++itr;
      return itr;
  }//Ivan:: note to self, in the system I am thinking of, would you actually do ith element = index or index * occurrence of each index??
	
  

  //7. length_unique: number of unique elements in your data object
  int length_unique() const {
      return unique.size();
  }
  // 8, length_total: total number of elements in your data object
  int length_total() const {
      return cnt;
  }
  
  int length() const {//virtual function length() from Link<E> must be overriden. deleting length() here will result in Visual Studios refusing to compile for LList<E> having un-overriden functions
	  return cnt; //size of linked list
  }
  //9. unique: return all the unique elements as an std::set
  std::set<E> unique_set() const {
      return unique;
  }

  /*******************************
   * 10. getters (all must be Θ(1)):
   */
  //10.1 get_mode: returns the mode value of the current entire data set
  std::vector<E> get_mode(){
      return count_el.modes;
  }
  // 10.2 get_median: returns the median value of the current entire data set
  double get_median(){
      if(cnt % 2 == 0)
          return (total[cnt/2] + total[cnt/2 -1])/2.0;
      return total[cnt/2];
  }
  // 10.3 get_ mean: returns the mean value of the current entire data set
  double get_mean(){
      return (double)(sum)/ cnt;
  }
  // 10.4 get_SD: returns the standard deviation  of the current entire data set
  double get_SD(){
      return sqrt((double)pre_SD /cnt);
  }
  // 10.5 get_min: returns the minimum data item in the current entire data set
  E get_min(){
      return total.front();
  }
  // 10.6 get_max: returns the maximum data item in the current entire data set //Ivan: wait, is it sorted ascending?
  E get_max(){
      return total.back();
  }


  //BELOW IS JUST LINKEDLIST MOVING CURR AROUND STUFF + getvalue() + reverse()

  // Place curr at list start (used in search)
  void moveToStart() {
    curr = head;
  }

  // Place curr at list end (can delete)
  void moveToEnd(){
    curr = tail;
  }
  // Move curr one step left; no change if already at front (can delete)
  void prev() {
    if (curr == head) return; // No previous element
    Link<E>* temp = head;
    // March down list until we find the previous element
    while (temp->next!=curr) temp=temp->next;
    curr = temp;
  }
  // Move curr one step right; no change if already at end (cannot delete)
  void next(){
    if (curr != tail) curr = curr->next;
  }
  // Return the position of the current element (can delete)
  int currPos() const {
    Link<E>* temp = head;
    int i;
    for (i=0; curr != temp; i++)
      temp = temp->next;
    return i;
  }
  // Move down list to "pos" position (can delete)
  void moveToPos(int pos) {
    Assert ((pos>=0)&&(pos<cnt), "Position out of range"); //changed pos <= cnt to pos < cnt, as cnt is one more than the index value
    curr = head;
    for(int i=0; i<pos; i++) curr = curr->next;
  }
  const E& getValue() const { // Return current element (used int search)
    Assert(curr->next != NULL, "No value");
    return curr->next->element;
  }
  //(can delete)
  void reverse(){
    LList<E> result(10);
    E it;
    for (int i = 0; i < 8; i++) {
        curr = head;

    //while (curr != NULL) {
        it = remove();
        result.insert(it);
        //cout << "Its in" << endl;

       }
    curr = head;
    this->head = result.head;
  }
  bool isAtEnd() { return curr ==tail; }

};

#endif // LLIST_H




*/
