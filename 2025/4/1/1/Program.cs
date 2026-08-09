// See https://aka.ms/new-console-template for more information

using System.Net.Mime;

var input = File.ReadAllLines("../../../../../input.txt");

if (input.Length == 0)
{
    Console.WriteLine("No input");
    return;
}

var result = 0;

for (var y = 0; y < input.Length; y++) 
{
    for (var x = 0; x < input[y].Length; x++)
    {
        if (input[y][x] != '@') continue;
        
        var neighbours = 0;
        for (var i = -1; i <= 1; i++)
        {
            if (y + i < 0 || y + i >= input.Length) continue;
            for (var j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0) continue;
                if (x + j < 0 || x + j >= input.Length) continue;

                if (input[y + i][x + j] == '@') neighbours++;
            }
        }

        if (neighbours < 4) result++;
    }
}

Console.WriteLine(result);