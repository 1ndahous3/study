
import java.math.BigInteger;
import java.util.AbstractMap;
import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.Random;
import java.util.Set;

public class SkipList<K extends Comparable<K>, V> extends AbstractMap<K, V> {
	ArrayList<ArrayList<SLPair>> lvl;
	int levels;
	SLPair emptyElement = new SLPair();

	public SkipList(int levels) {
		this.levels = levels;
		lvl = new ArrayList<ArrayList<SLPair>>();
		for (int i = 0; i < levels; i++) {
			lvl.add(new ArrayList<SLPair>());
			lvl.get(i).add(emptyElement);
		}
	}

	class SLPair {
		SLPair ptr = this;
		Map.Entry<K, V> Pair;

		SLPair(K key, V val) {
			Pair = new AbstractMap.SimpleEntry<>(key, val);
		}

		SLPair() {
		};

		private boolean isEmpty() {
			return this == emptyElement;
		}
	}

	@Override
	public boolean isEmpty() {
		return lvl.get(0).get(0).isEmpty();
	}

	public V remove(Object key) {
		ArrayList<SLPair> lst_cur = new ArrayList<>();
		int j = getIndex(key, lst_cur);
		if (lvl.get(0).get(j).Pair.getKey().compareTo((K) key) != 0)
			return null;
		V res = lvl.get(0).get(j).Pair.getValue();
		for (int i = 0; i < levels && !lvl.get(i).get(j).isEmpty(); i++) {
			if (j != 0)
				lst_cur.get(levels - i - 1).ptr = lst_cur.get(levels - i - 1).ptr.ptr;
			else if (lvl.get(i).get(j).ptr != lvl.get(i).get(j + 1)) {
				lvl.get(i).set(j + 1,
						new SLPair(lvl.get(j).get(j + 1).Pair.getKey(), lvl.get(j).get(j + 1).Pair.getValue()));
				lvl.get(i).get(j + 1).ptr = lvl.get(i).get(j).ptr;
			}
		}
		for (int i = 0; i < levels; i++)
			lvl.get(i).remove(j);
		return res;
	}

	public V put(K key, V val) {
		ArrayList<SLPair> lst_cur = new ArrayList<>();
		int j = getIndex(key, lst_cur);
		V res = null;
		if (!lvl.get(0).get(j).isEmpty() && lvl.get(0).get(j).Pair.getKey().compareTo(key) == 0)
			res = remove(key);
		if (j != 0) {
			for (int i = levels - 1;; i--) {
				BigInteger rnd = new BigInteger(i, new Random());
				if (rnd.compareTo(BigInteger.ZERO) == 0) {
					for (int k = i; k >= 0; k--) {
						lvl.get(k).add(j, new SLPair(key, val));
						lvl.get(k).get(j).ptr = lst_cur.get(levels - k - 1).ptr;
						lst_cur.get(levels - k - 1).ptr = lvl.get(k).get(j);
					}
					break;
				} else
					lvl.get(i).add(j, emptyElement);
			}
		} else {
			for (int k = levels - 1; k >= 0; k--) {
				lvl.get(k).add(j, new SLPair(key, val));
				lvl.get(k).get(j).ptr = lvl.get(k).get(1);
			}
		}
		return res;
	}

	@SuppressWarnings("unchecked")
	public V get(Object key) {
		int a = getIndex((K) key);
		SLPair res = lvl.get(0).get(a);
		return res.Pair != null && res.Pair.getKey().compareTo((K) key) == 0 ? res.Pair.getValue() : null;
	}

	@Override
	public void clear() {
		lvl = new ArrayList<ArrayList<SLPair>>();
		for (int i = 0; i < levels; i++) {
			lvl.add(new ArrayList<SLPair>());
			lvl.get(i).add(emptyElement);
		}
	}

	@Override
	public Set<Map.Entry<K, V>> entrySet() {
		return new AbstractSet<Map.Entry<K, V>>() {
			public Iterator<Map.Entry<K, V>> iterator() {
				return new Iterator<Map.Entry<K, V>>() {
					int count = 0;
					SLPair elem = lvl.get(0).get(0);

					public boolean hasNext() {
						elem = lvl.get(0).get(count);
						boolean res = count + 1 < lvl.get(0).size();
						count++;
						return res;
					}

					public Map.Entry<K, V> next() {
						return lvl.get(0).get(count - 1).Pair;
					}

					public void remove() {
						count--;
						SkipList.this.remove(elem.Pair.getKey());
					}
				};
			}

			@Override
			public int size() {
				return lvl.get(0).size() - 1;
			}
		};

	}

	private int getIndex(K key) {
		return getIndex(key, new ArrayList<SLPair>());
	}

	@SuppressWarnings("unchecked")
	private int getIndex(Object key, ArrayList<SLPair> list) {
		SLPair curr = lvl.get(levels - 1).get(0);
		for (int i = levels - 1; i > 0;) {
			list.add(emptyElement);
			while (!curr.isEmpty()) {
				if (!curr.ptr.isEmpty()) {
					if (curr.ptr.Pair.getKey().compareTo((K) key) >= 0) {
						list.set(levels - i - 1, curr);
						break;
					}
				} else {
					if (curr.Pair.getKey().compareTo((K) key) < 0)
						list.set(levels - i - 1, curr);
					break;
				}
				list.set(levels - i - 1, curr);
				curr = curr.ptr;
			}
			i--;
			int tmp = lvl.get(i + 1).indexOf(curr);
			curr = lvl.get(i).get(tmp);
		}
		while (!curr.isEmpty() && curr.Pair.getKey().compareTo((K) key) < 0)
			curr = curr.ptr;
		int rez = lvl.get(0).indexOf(curr);
		list.add(rez != 0 ? lvl.get(0).get(rez - 1) : emptyElement);
		return rez;
	}
}