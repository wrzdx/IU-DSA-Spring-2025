//Evgenii Khovalyg

import java.util.Scanner;

public class Main {
    public static int min(int a, int b) {
        return a > b ? b : a;
    }

    // Implementing partial insertion sort algorithm based on lab snippets
    public static int[] partialSortArr(int[] scoresArr, String[] namesArr, int k) {
        for (int i = 1; i < scoresArr.length; i++) {
            int currentScore = scoresArr[i];
            String currentName = namesArr[i];
            int j = min(k, i);
            while (j > 0 && scoresArr[j - 1] < currentScore) {
                scoresArr[j] = scoresArr[j - 1];
                namesArr[j] = namesArr[j - 1];
                j--;

            }
            scoresArr[j] = currentScore;
            namesArr[j] = currentName;
        }
        return scoresArr;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int k = scanner.nextInt();
        String[] namesArr = new String[n];
        int[] scoresArr = new int[n];
        for (int i = 0; i < n; i++) {
            namesArr[i] = scanner.next();
            scoresArr[i] = scanner.nextInt();
        }

        int[] partialSortedArr = partialSortArr(scoresArr, namesArr, k);

        for (int i = 0; i < min(n, k); i++) {
            System.out.println(namesArr[i] + " " + partialSortedArr[i]);
        }
    }
}
