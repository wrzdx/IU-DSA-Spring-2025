//Evgenii Khovalyg DSAI-02


import java.util.*;

public class Main {
    /**
     * Sort method
     * <p>
     * Performs a merge sort on the given ArrayList.
     * This implementation is written by me.
     * </p>
     *
     * @param arr the ArrayList to sort
     * @return a new sorted ArrayList of MapEntry objects
     */
    public static ArrayList<HashMap.MapEntry<String, Integer>> sort(
            ArrayList<HashMap.MapEntry<String, Integer>> arr) {
        // Base case: if the list has 0 or 1 element, it is already sorted.
        if (arr.size() <= 1) {
            return arr;
        }
        // Create two new lists to hold the left and right halves.
        ArrayList<HashMap.MapEntry<String, Integer>> left = new ArrayList<>();
        ArrayList<HashMap.MapEntry<String, Integer>> right = new ArrayList<>();
        int mid = arr.size() / 2;


        for (int i = 0; i < mid; i++) {
            left.add(arr.get(i));
        }
        for (int i = mid; i < arr.size(); i++) {
            right.add(arr.get(i));
        }

//      Divide the array until it will have size of 1
        left = sort(left);
        right = sort(right);

//      Merge the sorted halves.
        int lPointer = 0;
        int rPointer = 0;
        ArrayList<HashMap.MapEntry<String, Integer>> merged = new ArrayList<>();
        while (lPointer < left.size() && rPointer < right.size()) {
            HashMap.MapEntry<String, Integer> lEntry = left.get(lPointer);
            HashMap.MapEntry<String, Integer> rEntry = right.get(rPointer);

//          Comparing based on the order of appearance.
            if (rEntry.getValue() < lEntry.getValue()) {
                merged.add(rEntry);
                rPointer++;
            } else {
                merged.add(lEntry);
                lPointer++;
            }
        }
        // Append any remaining elements from the left list.
        while (lPointer < left.size()) {
            merged.add(left.get(lPointer));
            lPointer++;
        }
        while (rPointer < right.size()) {
            merged.add(right.get(rPointer));
            rPointer++;
        }
        return merged;
    }

    /**
     * Main method.
     * <p>
     * Reads input data to fill a HashMap, removes repeating words from it,
     * then sorts the remaining words based on the order of appearance,
     * and finally prints the sorted words.
     * </p>
     *
     * @param args command line arguments (not used)
     */
    public static void main(String[] args) {
        // Create an instance of our custom HashMap.
        HashMap<String, Integer> map = new HashMap<>();
        Scanner scanner = new Scanner(System.in);

        // Read the number of words to insert.
        int n = scanner.nextInt();
        String word;

        // For each word, if it is not already in the map, add it with its index,
        // because we need to print exactly k unique words
        for (int i = 0; i < n; i++) {
            word = scanner.next();
            if (map.get(word) == null) {
                map.put(word, i);
            }
        }
        int m = scanner.nextInt();

        // Read another m words and remove them from the map.
        for (int i = 0; i < m; i++) {
            word = scanner.next();
            map.remove(word);
        }

        // Create a list of remaining words.
        ArrayList<HashMap.MapEntry<String, Integer>> words = new ArrayList<>();
        for (Map.Entry<String, Integer> entry : map.entrySet()) {
            // Cast is safe here since our map stores MapEntry objects
            // and MapEntry implements interface Entry
            words.add((HashMap.MapEntry<String, Integer>) entry);
        }

        // Sort words by the order of appearance
        words = sort(words);

        // Print the number of remaining words and then each word.
        System.out.println(words.size());
        for (Map.Entry<String, Integer> leftWord : words) {
            System.out.println(leftWord.getKey());
        }
    }

    /**
     * Custom HashMap implementation using separate chaining for collision resolution.
     * <p>
     * This implementation is partially based on:
     * <ul>
     *   <li>The article "Implementing our own Hash Table with Separate Chaining in Java" from GeeksforGeeks
     *       (<a href="https://www.geeksforgeeks.org/implementing-our-own-hash-table-with-separate-chaining-in-java">
     *       https://www.geeksforgeeks.org/implementing-our-own-hash-table-with-separate-chaining-in-java</a>).</li>
     *   <li>The interfaces and MapEntry implementation from the book
     *       "Data Structures and Algorithms in Java, 6th Edition" (Wiley, 2014).</li>
     *   <li>The hash function is adapted from a video:
     *       <a href="https://youtu.be/cWbuK7C13HQ?t=538&si=-AzSeaf323CLb9-l">
     *       https://youtu.be/cWbuK7C13HQ?t=538&si=-AzSeaf323CLb9-l</a>.</li>
     * </ul>
     * Many aspects of this code have been simplified compared to the original sources.
     * Drawing valuable insights from more advanced implementations,
     * I have developed this solution largely on my own,
     * ensuring that a tailored implementation meets the project requirements.
     * </p>
     *
     * @param <K> key type
     * @param <V> value type
     */
    public static class HashMap<K, V> implements Map<K, V> {
        /**
         * Nested MapEntry class representing a key-value pair.
         *
         * @param <K> key type
         * @param <V> value type
         */
        // ---------------- Nested MapEntry Class ----------------
        public static class MapEntry<K, V> implements Entry<K, V> {
            private K key;  // Key
            private V value;  // Value

            public MapEntry(K key, V value) {
                this.key = key;
                this.value = value;
            }

            @Override
            public K getKey() {
                return key;
            }

            @Override
            public V getValue() {
                return value;
            }

            protected void setKey(K key) {
                this.key = key;
            }

            protected void setValue(V value) {
                this.value = value;
            }
        }
        // ---------------- End of Nested MapEntry Class ----------------

        private ArrayList<LinkedList<MapEntry<K, V>>> array;
        private int capacity;
        private int size;

        /**
         * Constructs a new HashMap with an initial capacity of 10.
         */
        public HashMap() {
            capacity = 10;
            size = 0;
            array = new ArrayList<>(capacity);
            for (int i = 0; i < capacity; ++i) {
                array.add(new LinkedList<>());
            }
        }

        @Override
        public int size() {
            return size;
        }

        @Override
        public boolean isEmpty() {
            return size == 0;
        }

        /**
         * Computes a hash value for the given key.
         * <p>
         * This method first converts the key to its string representation.
         * It then iterates over each character of the string and updates the hash value by multiplying
         * the current hash by 31 and adding the Unicode value of the character.
         * <br>
         * Finally, the method returns the absolute value of the computed hash to ensure it is non-negative.
         * </p>
         *
         * @param key the key for which to compute the hash
         * @return a non-negative hash value
         */
        private int getHash(K key) {
            if (key == null) {
                return 0;
            }
            String convertedToStringKey = key.toString();
            int hash = 0;

            for (int i = 0; i < convertedToStringKey.length(); ++i) {
                // The multiplication by 31 is performed using bit shifting:
                hash = (hash << 5) - hash + convertedToStringKey.charAt(i);
            }
            return Math.abs(hash);
        }

        /**
         * Computes the index in the array for the given key.
         *
         * @param key the key
         * @return the index in the underlying array
         */
        private int getIndex(K key) {
            int hash = getHash(key);
            // Make sure that hash is not bigger than capacity
            return hash % capacity;
        }

        /**
         * Returns the value associated with the specified key, or null if no mapping exists.
         *
         * @param key the key whose associated value is to be returned
         * @return the value associated with the specified key, or null if none exists
         */

        @Override
        public V get(K key) {
            int index = getIndex(key);
            LinkedList<MapEntry<K, V>> list = array.get(index);
            for (Entry<K, V> entry : list) {
                if ((key == null && entry.getKey() == null) ||
                        (key != null && key.equals(entry.getKey()))) {
                    return entry.getValue();
                }
            }
            return null;
        }

        /**
         * Associates the specified value with the specified key in this map.
         * If the key already exists, its value is updated.
         * Rehashing occurs if the load factor exceeds 0.7 after insertion.
         *
         * @param key   the key with which the specified value is to be associated
         * @param value the value to be associated with the specified key
         * @return the previous value associated with key, or null if there was no mapping
         */
        @Override
        public V put(K key, V value) {
            int index = getIndex(key);
            LinkedList<MapEntry<K, V>> list = array.get(index);
            for (MapEntry<K, V> entry : list) {
                if ((key == null && entry.getKey() == null) ||
                        (key != null && key.equals(entry.getKey()))) {
                    V oldValue = entry.getValue();
                    entry.setValue(value);
                    return oldValue;
                }
            }
            list.add(new MapEntry<>(key, value));
            size++;

            if ((1.0 * size) / capacity >= 0.7) {
                // Create new array, and reinsert every element from initial one
                ArrayList<LinkedList<MapEntry<K, V>>> temp = array;
                array = new ArrayList<>();
                capacity = 2 * capacity;
                size = 0;
                for (int i = 0; i < capacity; i++) {
                    array.add(new LinkedList<>());
                }
                for (LinkedList<MapEntry<K, V>> bucket : temp) {
                    for (MapEntry<K, V> entry : bucket) {
                        put(entry.getKey(), entry.getValue());
                    }
                }
            }

            return null;
        }

        /**
         * Removes the mapping for the specified key from this map if present.
         *
         * @param key the key whose mapping is to be removed
         * @return the previous value associated with the key, or null if no mapping existed
         */
        @Override
        public V remove(K key) {
            int index = getIndex(key);
            LinkedList<MapEntry<K, V>> list = array.get(index);
            Iterator<MapEntry<K, V>> iter = list.iterator();
            while (iter.hasNext()) {
                Entry<K, V> entry = iter.next();
                if ((key == null && entry.getKey() == null) ||
                        (key != null && key.equals(entry.getKey()))) {
                    V value = entry.getValue();
                    iter.remove();
                    size--;
                    return value;
                }
            }
            return null;
        }

        /**
         * Returns an Iterable view of the keys contained in this map.
         *
         * @return an Iterable of the keys in this map
         */
        @Override
        public Iterable<K> keySet() {
            List<K> keys = new ArrayList<>();
            for (LinkedList<MapEntry<K, V>> list : array) {
                for (MapEntry<K, V> entry : list) {
                    keys.add(entry.getKey());
                }
            }
            return keys;
        }

        /**
         * Returns an Iterable view of the values contained in this map.
         *
         * @return an Iterable of the values in this map
         */
        @Override
        public Iterable<V> values() {
            List<V> values = new ArrayList<>();
            for (LinkedList<MapEntry<K, V>> list : array) {
                for (Entry<K, V> entry : list) {
                    values.add(entry.getValue());
                }
            }
            return values;
        }

        /**
         * Returns an Iterable view of the key-value mappings contained in this map.
         *
         * @return an Iterable of the entries in this map
         */
        @Override
        public Iterable<Entry<K, V>> entrySet() {
            List<Entry<K, V>> entries = new ArrayList<>();
            for (LinkedList<MapEntry<K, V>> list : array) {
                entries.addAll(list);
            }
            return entries;
        }
    }


    /**
     * A generic Map interface that defines the basic operations for a key-value data structure.
     *
     * @param <K> the type of keys maintained by this map
     * @param <V> the type of mapped values
     */
    public interface Map<K, V> {

        /**
         * Represents a key-value pair (an entry) in the map.
         *
         * @param <K> the type of the key
         * @param <V> the type of the value
         */
        interface Entry<K, V> {
            /**
             * Returns the key corresponding to this entry.
             *
             * @return the key corresponding to this entry
             */
            K getKey();

            /**
             * Returns the value corresponding to this entry.
             *
             * @return the value corresponding to this entry
             */
            V getValue();
        }

        /**
         * Returns the number of key-value mappings in this map.
         *
         * @return the number of key-value mappings in this map
         */
        int size();

        /**
         * Returns {@code true} if this map contains no key-value mappings.
         *
         * @return {@code true} if this map is empty; {@code false} otherwise
         */
        boolean isEmpty();

        /**
         * Returns the value to which the specified key is mapped,
         * or {@code null} if this map contains no mapping for the key.
         *
         * @param key the key whose associated value is to be returned
         * @return the value associated with the specified key, or {@code null} if no mapping exists
         */
        V get(K key);

        /**
         * Associates the specified value with the specified key in this map.
         * If the map previously contained a mapping for the key, the old value is replaced.
         *
         * @param key   the key with which the specified value is to be associated
         * @param value the value to be associated with the specified key
         * @return the previous value associated with the key, or {@code null} if there was no mapping
         */
        V put(K key, V value);

        /**
         * Removes the mapping for the specified key from this map if present.
         *
         * @param key the key whose mapping is to be removed
         * @return the value that was associated with the key, or {@code null} if no mapping existed
         */
        V remove(K key);

        /**
         * Returns an {@code Iterable} over the keys contained in this map.
         *
         * @return an {@code Iterable} of the keys in this map
         */
        Iterable<K> keySet();

        /**
         * Returns an {@code Iterable} over the values contained in this map.
         *
         * @return an {@code Iterable} of the values in this map
         */
        Iterable<V> values();

        /**
         * Returns an {@code Iterable} over the key-value entries contained in this map.
         *
         * @return an {@code Iterable} of the entries in this map
         */
        Iterable<Entry<K, V>> entrySet();
    }

}
