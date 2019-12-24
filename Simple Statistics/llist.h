#ifndef LLIST_H
#define LLIST_H

// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

//Ivan: Ivan read this document, did not edit Ryusei's functions/variables. Added comments to suggest to do nexts. Ivan is still reading all the code and will edit the code once all the code and requirements are memorized.
#include "list.h"
#include "link.h"
#include <iostream>

#include <vector>
#include <set>
#include <math.h> //Ivan: I would like to politely add that "8.	You are not allowed to use any C/C++ math libraries"

// Linked list implementation
template <typename E> 
class LList: public List<E> {
private:
  Link<E>* head; // Pointer to list header
  Link<E>* tail; // Pointer to last element
  Link<E>* curr; // Access to current element

  int cnt; // Size of list
  int sum; // sum of elements //Ivan: new

  double pre_SD; // sum of (x - aveX)*(x - aveX) //Ivan: so this is the numerator of the variance

  std::set<E> unique; //unique elements //for submitting the set of unique elements when calling std::set unique_set() {}
  std::vector<E> total; //Ivan: new (compared to source file, this was newly added by Ryusei)

  void init() { // Initialization helper method //note has issue of empty head
    curr = tail = head = new Link<E>;
    cnt = 0;
    sum = 0;
  }

  void removeall() { // Return link nodes to free store
    while(head != NULL) {
        curr = head;
        head = head->next;
        delete curr;
      }
    unique.clear(); //Ivan: new
    total.clear(); //Ivan: new
  }


  //Ivan: el_num as in el numero? What is this for? Also, never had seen a struct inside a class like this before. should be above the class. 
  //Why is the struct instantiated inside the class not inside the constructor?
  //
  struct el_num{ 
      std::vector<E> element;
      int count = 0;
  };
  el_num count_el;

  /********************************
   * here is some problem
   */
  void max_count(E target){ //Ivan: what is this supposed to do?
      for(E s: unique){
          if(search(s).size() == count_el.count){
              count_el.element.push_back(s);
          }else if(search(s).size() > count_el.count){
              count_el.element.clear();
              count_el.element.push_back(s);
              count_el.count = search(s).size();
          }
      }
  }
  void total_sort(){//Ivan: this might be worse than using inserting into a vector or keeping track of the max and min by a simple comparison whenever something is inserted. Such comparison can even be skipped if we know new element is not unique.
      sort(total.begin(), total.end());
  }

  void pre_SD_sum(){ //Ivan: why not go stread to sd? also pow() is not allowed
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

  //Ivan: we lost the print() of list. While such is not required, I might need to import it from my llist.h later for dev reasons i.e. checking if nothing went wrong. 
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
      max_count(it);
      pre_SD_sum();
  }

  // Insert "it" at current position (used in append) //Ivan: insert() is unchanged from source
  void insert(const E& it) {
	  std::cout << "Its in" << std::endl;

	  curr->next = new Link<E>(it, curr->next);
	  if (tail == curr) tail = curr->next; // New tail
	  cnt++;
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

      total.erase(total.begin() + search(it)[0] -1);
      if(this->search(it).size() == 0)
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

  // 5. search (fastest possible among the choices what we have  discussed in the class):
  // must return the location of the first occurrence and repetition.
  // For example, if the first occurrence of data value 20 is 3  and there are 10 of 20s,
  // then you must return 3 and 10 as the search result of 20
  std::vector<int> search(int target){
      std::vector<int> result;

      this->moveToStart();
      for(int i=0; i < this->cnt; i++){
          if(this->getValue() == target){
              result.push_back(i);
              this->next();
          }
      }
      return result; //Ivan: does this return a destroyed, out of scope vector? 
	  //Also, the question does not request all the occurances, just the index location of the first occurance. So if you are going this approach, you can return a std::pair pairname(index location, frequency count)
	  //but the ideal solution is to just store values in std::pairs of (value, occurance) anyways?
  }
  // 6. array index operator, []: access each unique element through an index:
  // overload the array index operator ([]) so that the ith data element in an instance of your data object,
  // db can be accessed like db[i]
  std::set<int> operator[](int pos){
      auto itr = unique.begin();
      for(int i = 0; i < pos; ++i)//Should "<" be here or should it be "<="? Usually when I ask for set[0], I want position 0, but this here would stop at i < 0//or is this a response to the empty head paradoxes?
          ++itr;
      return itr;
  }//Ivan:: note to self, in the system I am thinking of, would you actually do ith element = index or index * occurance of each index??

  

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
      return count_el.element;
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
    //Assert ((pos>=0)&&(pos<=cnt), "Position out of range");
    curr = head;
    for(int i=0; i<pos; i++) curr = curr->next;
  }
  const E& getValue() const { // Return current element (used int search)
    //Assert(curr->next != NULL, "No value");
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
