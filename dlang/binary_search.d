import
    std.stdio,
    std.conv;

template binsearch(string compare_sym) {
    static assert(compare_sym == ">" || compare_sym == "<", ` '<' (ascending) or '>' (descending) expected `);

    static if (compare == "<") {
        enum dir_compare = "<";
        enum rev_compare = ">";
    } else {
        enum dir_compare = ">";
        enum rev_compare = "<";
    }

    enum binsearch =
    `function (int[] arr, int elem) {
        int mid, min = 0, max = arr.length.to!int - 1, res = -1;
        while (min <= max) {
        	mid = (min + max) / 2;
            if (arr[mid]` ~ dir_compare ~ `elem) {
                min = mid + 1;
            } else if (arr[mid]` ~ rev_compare ~ `elem) {
                max = mid - 1;
            } else {
                res = mid;
                break;
            }
        }
        return res;
    }`;
}

void main(string[] args)
{
    int[] a = [1, 2, 3, 4, 5];
    int[] b = [5, 4, 3, 2, 1];

    auto ascending_binsearch = mixin(binsearch!("<"));
    auto descending_binsearch = mixin(binsearch!(">"));

    ascending_binsearch(a, 4).writeln();
    descending_binsearch(b, 3).writeln();
    descending_binsearch(b, 7).writeln();
}
