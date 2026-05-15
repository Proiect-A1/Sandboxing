using System;
using System.IO;

class Program {
    static void Main() {
        string[] dirs = Directory.GetFileSystemEntries(".");
        foreach (string dir in dirs) {
            Console.Error.WriteLine(dir);
        }
    }
}