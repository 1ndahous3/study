import java.util.AbstractSet;
import java.util.Iterator;

public class IntSparseSet extends AbstractSet<Integer> {
	int sparse[], dense[], num = 0, shift;

	public IntSparseSet(int low, int high) {
		shift = low;
		sparse = new int[high - low];
		dense = new int[high - low];
	}

	@Override
	public boolean add(Integer a) {
		if (!(a - shift < sparse.length && a - shift >= 0) || contains(a))
			return false;
		dense[num] = a;
		sparse[dense[num] - shift] = num;
		num++;
		return true;
	}

	public boolean contains(Integer a) {
		return (a - shift < sparse.length && (0 <= sparse[a - shift] && sparse[a - shift] < num)
				&& dense[sparse[a - shift]] == a);
	}

	public boolean remove(Integer a) {
		if (!(a - shift < sparse.length && a - shift >= 0) || !contains(a))
			return false;
		num--;
		dense[sparse[a - shift]] = dense[num];
		sparse[dense[num] - shift] = sparse[a - shift];
		return true;
	}

	public int size() {
		return num;
	}

	public void clear() {
		num = 0;
	}

	public Iterator<Integer> iterator() {
		Iterator<Integer> res = new Iterator<Integer>() {
			private int i = -1;

			public boolean hasNext() {
				return i < num - 1;
			}

			public Integer next() {
				i++;
				return dense[i];
			}

			public void remove() {
				IntSparseSet.this.remove(dense[i]);
			}
		};
		return res;
	}
}