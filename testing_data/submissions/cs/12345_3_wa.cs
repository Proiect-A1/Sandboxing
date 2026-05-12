using System;

class Program {
    static void Main() {
        string[] tokens = Console.In.ReadToEnd().Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
        if (tokens.Length == 0) return;
        Console.WriteLine(int.Parse(tokens[0]) + 1);
    }
}