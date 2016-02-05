import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Iterator;

public class SparseSet<T extends Hintable> extends AbstractSet<T> {
	ArrayList<T> dense;
	int num = 0;

	public SparseSet() {
		dense = new ArrayList<T>();
	}

	@Override
	public boolean add(T a) {
		if (contains(a))
			return false;
		a.setHint(num++);
		dense.add(a);
		return true;
	}

	public boolean contains(T a) {
		return (dense.size() > a.hint() && dense.get(a.hint()) == a);
	}

	public boolean remove(T a) {
		if (!contains(a))
			return false;
		dense.get(--num).setHint(a.hint());
		dense.set(a.hint(), dense.get(num));
		dense.remove(num);
		return true;
	}

	public int size() {
		return num;
	}

	public void clear() {
		num = 0;
	}

	public Iterator<T> iterator() {
		Iterator<T> res = new Iterator<T>() {
			private int i = -1;

			public boolean hasNext() {
				return i < num - 1;
			}

			public T next() {
				return dense.get(++i);
			}

			public void remove() {
				SparseSet.this.remove(dense.get(i));
			}
		};
		return res;
	}
}