import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.stream.Collectors;

public class HashMapInverse {
    public static void main(String[] args) {
        Map<Integer, String> myMap = new HashMap<>();
        Scanner sn = new Scanner(System.in);
        int n = sn.nextInt();
        for (int i = 0; i < n; i++) {
            myMap.put(sn.nextInt(), sn.nextLine());
        }
        Map<String, Integer> invMap = myMap.entrySet()
                                                        .stream()
                                                        .collect(Collectors.groupingBy(Map.Entry::getValue)).entrySet()
                                                        .stream()
                                                        .filter(e -> e.getValue().size() == 1)
                                                        .collect(Collectors.toMap(Map.Entry::getKey, e -> e.getValue().get(0).getKey()));
        System.out.println(invMap.get("string"));
    }
}
