import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

enum tags { IDENT, NUMBER, LPAREN, RPAREN, END, ADD, SUB, MUL, DIV };

@SuppressWarnings("serial")
class ParseErrorException extends Exception {
	int pos = 0;
	
	public ParseErrorException() {};
	public ParseErrorException(int a) { pos = a; }
	
}

class Node {
	tags tag;
	int num;
	String ident;

	Node(tags tag) {
		this.tag = tag;
	}

	Node(tags tag, int num) {
		this.tag = tag;
		this.num = num;
	}

	Node(tags tag, String ident) {
		this.tag = tag;
		this.ident = ident;
	}
}

public class Calc {
	private int curr = 0, next = 0;
	private String str;
	private Scanner in;
	private tags tag;
	private HashMap<String, Integer> idents;

	private int parse() throws ParseErrorException {
		ArrayList<Node> foo = new ArrayList<>();
		this.next_token(foo);
		int res = this.parse_expr(foo);
		if (!this.tag.equals(tags.END))
			throw new ParseErrorException();
		return res;
	}

	private int next_char() {
		this.curr = this.next;
		if (this.curr == this.str.length())
			return -1;
		return this.str.charAt(this.next++);
	}

	private void next_token(ArrayList<Node> bar) throws ParseErrorException {
		int c;
		while (Character.isSpaceChar(c = next_char()))
			;
		if (Character.isAlphabetic(c)) {
			int foo = this.curr;
			do {
				c = this.next_char();
			} while (Character.isAlphabetic(c) || (Character.isDigit(c) && c != -1));
			String substr = str.substring(foo, this.curr);
			this.tag = tags.IDENT;
			if (!idents.containsKey(substr))
				idents.put(substr, this.in.nextInt());
			if (c != -1)
				this.next--;
			bar.add(new Node(tags.NUMBER, idents.get(substr)));
		} else if (Character.isDigit(c) && c != -1) {
			int foo = this.curr;
			while (Character.isDigit(c = next_char()))
				;
			this.tag = tags.NUMBER;
			if (c != -1)
				this.next--;
			bar.add(new Node(tag, Integer.parseInt(str.substring(foo, this.curr))));
		} else {
			switch (c) {
			case -1:
				tag = tags.END;
				break;
			case '(':
				tag = tags.LPAREN;
				break;
			case ')':
				tag = tags.RPAREN;
				break;
			case '+':
				tag = tags.ADD;
				bar.add(new Node(tag));
				break;
			case '-':
				tag = tags.SUB;
				bar.add(new Node(tag));
				break;
			case '*':
				tag = tags.MUL;
				bar.add(new Node(tag));
				break;
			case '/':
				tag = tags.DIV;
				bar.add(new Node(tag));
				break;
			default:
				throw new ParseErrorException(curr);
			}
		}
	}

	@SuppressWarnings("incomplete-switch")
	private int parse_expr(ArrayList<Node> foo) throws ParseErrorException {
		while (tag.equals(tags.IDENT) || tag.equals(tags.NUMBER) || tag.equals(tags.LPAREN) || tag.equals(tags.ADD)
				|| tag.equals(tags.SUB) || tag.equals(tags.MUL) || tag.equals(tags.DIV)) {
			parse_term(foo);
		}
		for (int i = foo.size() - 1; i >= 0; i--)
			if (foo.get(i).tag.equals(tags.SUB)) {
				foo.get(i + 1).num = -foo.get(i + 1).num;
				if (i != 0 && foo.get(i - 1).tag.equals(tags.NUMBER))
					foo.get(i).tag = tags.ADD;
				else
					foo.remove(i);
				continue;
			}
		for (int i = 0; i < foo.size(); i++)
			switch (foo.get(i).tag) {
			case MUL:
				foo.get(i - 1).num *= foo.get(i + 1).num;
				foo.remove(i);
				foo.remove(i--);
				break;
			case DIV:
				foo.get(i - 1).num /= foo.get(i + 1).num;
				foo.remove(i);
				foo.remove(i--);
				break;
			}
		for (int i = 0; i < foo.size(); i++)
			switch (foo.get(i).tag) {
			case ADD:
				foo.get(i - 1).num += foo.get(i + 1).num;
				foo.remove(i);
				foo.remove(i--);
				break;
			}
		return foo.get(0).num;
	}

	@SuppressWarnings("incomplete-switch")
	private void parse_term(ArrayList<Node> foo) throws ParseErrorException {
		switch (this.tag) {
		case NUMBER:
		case IDENT:
			this.next_token(foo);
			if (this.tag.equals(tags.NUMBER) || this.tag.equals(tags.IDENT) || this.tag.equals(tags.LPAREN))
				throw new ParseErrorException(curr);
			break;
		case LPAREN:
			ArrayList<Node> bar = new ArrayList<>();
			this.next_token(bar);
			foo.add(new Node(tags.NUMBER, this.parse_expr(bar)));
			if (!this.tag.equals(tags.RPAREN))
				throw new ParseErrorException(curr);
			this.next_token(foo);
			break;
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			this.next_token(foo);
			if (!this.tag.equals(tags.IDENT) && !this.tag.equals(tags.NUMBER) && !this.tag.equals(tags.LPAREN)
					&& !this.tag.equals(tags.SUB))
				throw new ParseErrorException(curr);
			break;
		}
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		Calc res = new Calc(in);
		try {
			System.out.println(res.parse());
		} catch (ParseErrorException err) {
			System.out.print("error");
			// System.out.println(" in position: " + err.pos);
		}
	}

	Calc(Scanner in) {
		this.in = in;
		this.str = in.nextLine();
		this.idents = new HashMap<>();
	}
}
