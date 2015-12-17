#ifndef UNDO_OBJECT_HPP_
#define UNDO_OBJECT_HPP_

#include <vector>
#include <iterator>
#include <ctime>

template<class Object>
class undo_object {

protected:
	enum Actions {};

	typedef struct {} event;

	std::vector<event> *undo_list, *redo_list;
	size_t buff_count;

public:

	/*template <class Event>
	typename std::vector<Event>::iterator begin();

	template <typename Event>
	typename std::vector<Event>::iterator end();*/

	virtual size_t size() = 0;
	virtual size_t max_size() = 0;
	virtual bool empty() = 0;

	undo_object() {
		undo_list = new std::vector<event>();
		redo_list = new std::vector<event>();
	}
	virtual ~undo_object() {
		delete undo_list;
		delete redo_list;
	}

	virtual bool isUndoListEmpty() = 0;
	virtual bool isRedoListEmpty() = 0;

	/*virtual Event undo() = 0;
	virtual event redo() = 0;

	virtual bool isTimeOfEventExpired();
	virtual bool isCountOfEventExpired();*/

	virtual int push_diffs(const Object &obj1, const Object &obj2) = 0;

};

#endif /* UNDO_OBJECT_HPP_ */
