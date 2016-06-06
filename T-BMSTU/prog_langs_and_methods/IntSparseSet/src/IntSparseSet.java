import java.util.AbstractSet;
import java.util.Iterator;

public class IntSparseSet extends AbstractSet<Integer> {
	private int sparse[], dense[], num = 0, shift;

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
		sparse[dense[num] - shift] = num++;
		return true;
	}

	public boolean contains(Integer a) {
		return (a - shift < sparse.length && (0 <= sparse[a - shift] && sparse[a - shift] < num)
				&& dense[sparse[a - shift]] == a);
	}

	public boolean remove(Integer a) {
		if (!(a - shift < sparse.length && a - shift >= 0) || !contains(a)) {
			return false;
		}
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
		return new Iterator<Integer>() {
			private int i = -1;

			@Override
			public boolean hasNext() {
				return i < num - 1;
			}

			@Override
			public Integer next() {
				return dense[++i];
			}

			@Override
			public void remove() {
				IntSparseSet.this.remove(dense[i]);
			}
		};
	}
}