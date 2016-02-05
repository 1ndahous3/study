public class Element<T> {
    public Element<T> pntr;
    private T val;
    
    public Element(T x) {
        pntr = this;
        val = x;
    }
    
    public T x() {
        return val;
    }
    
    public boolean equivalent(Element<T> elem) {
        return (this.GetTop().equals(elem.GetTop()));
    }
        
    public void union(Element<T> elem) {
        this.GetTop().pntr = elem.GetTop();
    }
    
    private Element<T> GetTop() {
    	return pntr.equals(this) ? pntr : (pntr = pntr.GetTop());
	}
}
