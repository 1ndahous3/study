#ifndef UNDO_CONTAINER_HPP_
#define UNDO_CONTAINER_HPP_

#include "undo_object.hpp"
#include <limits>

template<class Container>
class undo_container: public undo_object<Container> {

protected:
	enum Actions { add, del	};

	typedef struct {
		Actions type;
		unsigned int pos, len;
		Container obj;
	} event;

	typedef typename std::vector<event> vector;
	typedef typename Container::const_iterator const_iterator;
	typedef typename Container::const_reverse_iterator const_reverse_iterator;

	std::vector<event> *undo_list, *redo_list;

public:
	undo_container() {
		undo_list = new vector();
		redo_list = new vector();
	}

	~undo_container() {
		delete undo_list;
		delete redo_list;
	}

	typename vector::iterator begin() {
		return undo_list->begin();
	}

	typename vector::iterator end() {
		return undo_list->end();
	}

	size_t size() {
		return undo_list->size();
	}

	bool empty() {
		return undo_list->empty();
	}

	size_t max_size() {
		return std::numeric_limits<typename vector::size_type>::max();
	}

	event undo() {
		event foo = undo_list->back();
		redo_list->push_back(foo);
		return foo;
	}

	event redo() {
		event foo = redo_list->back();
		undo_list->push_back(foo);
		return foo;
	}

	bool isUndoListEmpty() {
		return undo_list->empty();
	}

	bool isRedoListEmpty() {
		return undo_list->empty();
	}

	//returns number of changes. 0 - objects are equivalent
	int push_diffs(const Container &obj1, const Container &obj2) {
		int num_of_changes = 0, lpos = 0;
		const_iterator it1_left, it2_left;
		const_reverse_iterator it1_right, it2_right;

		//check left border of difference
		for (it1_left = obj1.cbegin(), it2_left = obj2.cbegin();;
				++lpos, ++it1_left, ++it2_left) {
			//check if end of obj1
			if (it1_left == obj1.cend()) {
				//check if end of obj2
				if (it2_left != obj2.cend())
					break;
				else
					return 0;
			}

			if (it2_left == obj2.cend() || *it1_left != *it2_left)
				break;
		}

		//check right border of difference
		for (it1_right = obj1.crbegin(), it2_right = obj2.crbegin();;
				++it1_right, ++it2_right) {

			if ((it1_right == obj1.crend() && it2_right != obj2.crend())
					|| it2_right == obj2.crend())
				break;

			if (*it1_right != *it2_right) {
				it1_right--;
				it2_right--;
				break;
			} else if (it1_left == it1_right.base() - 1
					|| it2_left == it2_right.base() - 1)
				break;
		}
		event foo;
		foo.pos = lpos;

		if (std::distance(it1_left, it1_right.base() - 1) > 0) {
			foo.obj = Container(it1_left, it1_right.base() - 1);
			foo.type = Actions::del;
			foo.len = foo.obj.size();
			num_of_changes++;
			undo_list->push_back(foo);
		}
		if (std::distance(it2_left, it2_right.base() - 1) > 0) {
			foo.obj = Container(it2_left, it2_right.base() - 1);
			foo.type = Actions::add;
			foo.len = foo.obj.size();
			num_of_changes++;
			undo_list->push_back(foo);
		}
		return num_of_changes;
	}

};

#endif /* UNDO_CONTAINER_HPP_ */
