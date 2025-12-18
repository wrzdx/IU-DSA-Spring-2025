//Evgenii Khovalyg

import java.util.Scanner;

public class Main {
    // Implementing insertion sort algorithm from lab snippets
    public static int[] sortArr(int[] arr) {
        for (int i = 1; i < arr.length; i++) {
            int currentElement = arr[i];
            int j = i;
            while (j>0 && arr[j-1] > currentElement) {
                arr[j] = arr[j-1];
                j--;

            }
            arr[j] = currentElement;
        }
        return arr;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int[] initialArr = new int[n];
        for (int i = 0; i < n; i++) {
            initialArr[i] = scanner.nextInt();
        }

        int[] sortedArr = sortArr(initialArr);

        for (int i = 0; i < n; i++) {
            System.out.print(sortedArr[i] + " ");
        }
    }
}
