// Evgenii Khovalyg


import java.util.Scanner;

public class Main {
    interface Stack<T> {

        /**
         * Pushes an element onto the stack.
         *
         * @param value the value to be pushed onto the stack.
         */
        void push(T value);

        /**
         * Removes and returns the top element of the stack.
         *
         * @return the top element of the stack.
         */
        T pop();

        /**
         * Returns the top element of the stack without removing it.
         *
         * @return the top element of the stack.
         */
        T peek();

        /**
         * Returns the number of elements in the stack.
         *
         * @return the size of the stack.
         */
        int size();

        /**
         * Checks if the stack is empty.
         *
         * @return {@code true} if the stack is empty, otherwise {@code false}.
         */
        boolean isEmpty();
    }

    /**
     * A custom implementation of a dynamically resizable array.
     *
     * @param <T> the type of elements stored in the array.
     */
    public static class ArrayList<T> {
        private Object[] array; // Internal array to store elements
        private int size; // Current number of elements
        private int capacity; // Maximum capacity before resizing

        /**
         * Default constructor initializing the array with default capacity.
         */
        public ArrayList() {
            capacity = 10;
            array = new Object[capacity];
            size = 0;
        }

        /**
         * Adds an element to the array, resizing if necessary.
         *
         * @param value the element to be added.
         */
        public void add(T value) {
            if (size == capacity) {
                increaseCapacity(); // Expand capacity if full
            }
            array[size++] = value;
        }

        /**
         * Retrieves an element at the specified index.
         *
         * @param index the position of the element to retrieve.
         * @return the element at the given index.
         * @throws IndexOutOfBoundsException if index is out of range.
         */
        public T get(int index) {
            if (index < 0 || index >= size) {
                throw new IndexOutOfBoundsException("Index out of bounds");
            }
            return (T) array[index];
        }

        /**
         * Removes an element at the specified index, shifting subsequent elements.
         *
         * @param index the position of the element to remove.
         * @return the removed element.
         * @throws IndexOutOfBoundsException if index is out of range.
         */
        public T remove(int index) {
            if (index < 0 || index >= size) {
                throw new IndexOutOfBoundsException("Index out of bounds");
            }
            T removed = this.get(index);
            for (int i = index; i < size - 1; i++) {
                array[i] = array[i + 1]; // Shift elements left
            }
            array[--size] = null; // Clear last element
            return removed;
        }

        /**
         * Returns the current number of elements in the array.
         *
         * @return the size of the array.
         */
        public int size() {
            return size;
        }

        /**
         * Checks if the array is empty.
         *
         * @return {@code true} if the array has no elements, otherwise {@code false}.
         */
        public boolean isEmpty() {
            return size == 0;
        }

        /**
         * Doubles the capacity of the array when it reaches full capacity.
         */
        private void increaseCapacity() {
            capacity *= 2;
            Object[] newArray = new Object[capacity];
            System.arraycopy(array, 0, newArray, 0, size);
            array = newArray;
        }
    }

    /**
     * A stack implementation using a custom dynamic array.
     *
     * @param <T> the type of elements stored in the stack.
     */
    public static class ArrayStack<T> implements Stack<T> {
        private final ArrayList<T> stack;
        private int size;

        /**
         * Constructs an empty stack.
         */
        public ArrayStack() {
            stack = new ArrayList<>();
            size = 0;
        }

        /**
         * Pushes an element onto the stack.
         *
         * @param value the element to be pushed onto the stack.
         */
        @Override
        public void push(T value) {
            stack.add(value);
            size++;
        }

        /**
         * Removes and returns the top element of the stack.
         *
         * @return the top element of the stack.
         * @throws RuntimeException if the stack is empty.
         */
        @Override
        public T pop() {
            if (this.isEmpty()) {
                throw new RuntimeException("The stack is empty");
            }
            size--;
            return stack.remove(size);
        }

        /**
         * Returns the top element of the stack without removing it.
         *
         * @return the top element of the stack.
         * @throws RuntimeException if the stack is empty.
         */
        @Override
        public T peek() {
            if (this.isEmpty()) {
                throw new RuntimeException("The stack is empty");
            }
            return stack.get(size - 1);
        }

        /**
         * Returns the number of elements in the stack.
         *
         * @return the size of the stack.
         */
        @Override
        public int size() {
            return size;
        }

        /**
         * Checks if the stack is empty.
         *
         * @return {@code true} if the stack is empty, otherwise {@code false}.
         */
        @Override
        public boolean isEmpty() {
            return size == 0;
        }
    }


    /**
     * Checks if a given string consists only of numeric digits.
     *
     * @param token the string to be checked.
     * @return {@code true} if the string contains only digits, otherwise {@code false}.
     */
    public static boolean isNumber(String token) {
        for (int i = 0; i < token.length(); i++) {
            char c = token.charAt(i);
            if (c > '9' || c < '0') {
                return false;
            }
        }
        return true;
    }

    /**
     * Determines the precedence of mathematical operators.
     *
     * @param token the operator as a string.
     * @return the precedence level of the operator, where higher values indicate higher precedence.
     * Returns -1 if the operator is not recognized.
     */
    public static int get_precedence(String token) {
        return switch (token.charAt(0)) {
            case '+', '-' -> 1;
            case '*', '/' -> 2;
            case 'm' -> 3;
            default -> -1;
        };
    }

    /**
     * Applies a mathematical operation based on the operator popped from the stack.
     *
     * @param stack  the stack containing operators.
     * @param result the stack containing numerical values for computation.
     * @return the computed result of the operation.
     * @throws RuntimeException if there are not enough operands or there is no operation.
     */
    public static int applyOperation(ArrayStack<String> stack, ArrayStack<Integer> result) {
        Integer b = result.pop();
        Integer a = result.pop();
        String op = stack.pop();
        return switch (op.charAt(0)) {
            case '+' -> a + b;
            case '-' -> a - b;
            case '*' -> a * b;
            case '/' -> a / b;
            default -> (op.equals("max") ? (a > b ? a : b) : (a > b ? b : a));
        };
    }

    /**
     * Evaluates an arithmetic expression represented as an array of tokens.
     * Uses two stacks: one for operators and one for operands.
     *
     * @param expression an array of strings representing the expression tokens.
     * @return the computed result of the expression.
     */
    public static int getResult(String[] expression) {
        ArrayStack<String> stack = new ArrayStack<>();
        ArrayStack<Integer> result = new ArrayStack<>();

        for (String token : expression) {
            if (isNumber(token)) {
                result.push((token.charAt(0) - '0'));
            } else {
                if (token.equals("min") || token.equals("max") || token.equals("(")) {
                    stack.push(token);
                } else if (token.equals(")") || token.equals(",")) {
                    while (!stack.isEmpty() && !stack.peek().equals("(")) {
                        result.push(applyOperation(stack, result));
                    }
                    if (!stack.isEmpty() && token.equals(")")) stack.pop();
                } else {
                    while (!stack.isEmpty() && get_precedence(token) <= get_precedence(stack.peek())) {
                        result.push(applyOperation(stack, result));
                    }
                    stack.push(token);
                }
            }
        }

        while (!stack.isEmpty()) {
            Integer a = applyOperation(stack, result);
            result.push(a);
        }
        return result.pop();
    }

    /**
     * Reads an infix expression from user input,
     * converts it to postfix notation, evaluate it,
     * and prints the result.
     *
     * @param args command-line arguments (not used).
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String expression = scanner.nextLine();
        System.out.println(getResult(expression.split(" ")));
    }
}