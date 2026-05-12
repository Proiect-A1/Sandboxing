using System;

class Program {
    static void Main() {
        string[] tokens = Console.In.ReadToEnd().Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
        if (tokens.Length == 0) return;
        
        int x = int.Parse(tokens[0]);
        int size = Math.Max(100, x * 10 * 1024 * 1024);
        byte[] v = new byte[size];
        for (int i = 0; i < size; i += 100) {
            v[i] = (byte)(i % 256);
        }
        Console.WriteLine(v[size / 2]);
    }
}