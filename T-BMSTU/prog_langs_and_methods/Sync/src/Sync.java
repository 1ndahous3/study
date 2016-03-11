import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;

public class Sync {
	private static ArrayList<String> DelList, CopyList;

	public static void main(String[] args) throws IOException {
		File S = new File(args[0]);
		File D = new File(args[1]);
		DelList = new ArrayList<>();
		CopyList = new ArrayList<>();

		CompareFolder(S, D);

		if (CopyList.isEmpty() && DelList.isEmpty())
			System.out.println("IDENTICAL");
		else {
			DelList
					.stream()
					.sorted()
					.forEach(i -> System.out.println("DELETE " + i.substring(args[1].length() + 1)));
			CopyList
					.stream()
					.sorted()
					.forEach(i -> System.out.println("COPY " + i.substring(args[0].length() + 1)));
		}
	}

	private static void CompareFolder(File obj1, File obj2) throws IOException {
		if (obj1.exists())
			for (String i : obj1.list()) {
				File tmp1 = new File(obj1, i);
				File tmp2 = new File(obj2, i);
				if (tmp1.isDirectory())
					CompareFolder(tmp1, tmp2);
				else
					CompareFile(tmp1, tmp2);
			}
		if (obj2.exists())
			for (String i : obj2.list()) {
				File tmp1 = new File(obj1, i);
				File tmp2 = new File(obj2, i);
				if (!tmp1.exists()) {
					if (tmp2.isDirectory())
						CompareFolder(tmp1, tmp2);
					else
						DelList.add(tmp2.getPath());
				}
			}
	}

	private static void CompareFile(File obj1, File obj2) throws IOException {
		if (!obj2.exists())
			CopyList.add(obj1.getPath());
		else if (obj1.lastModified() != obj2.lastModified()) {
			byte[] a = Files.readAllBytes(Paths.get(obj1.getAbsolutePath()));
			byte[] b = Files.readAllBytes(Paths.get(obj2.getAbsolutePath()));
			if (!Arrays.equals(a, b)) {
				DelList.add(obj2.getPath());
				CopyList.add(obj1.getPath());
			}
		}
	}
}