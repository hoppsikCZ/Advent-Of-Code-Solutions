using System.Numerics;

var input = File.ReadAllLines("../../../input.txt");

var points = new Vector2[input.Length];

for (var i = 0; i < input.Length; i++)
{
    var coords = input[i].Split(',');
    points[i] = new Vector2(int.Parse(coords[0]), int.Parse(coords[1]));
}

ulong largest = 0;

for (var i = 0; i < points.Length - 1; i++)
{
    for (var j = i + 1; j < points.Length; j++)
    {
        var area = CalculateArea(points[i], points[j]);
        if (area > largest)
        {
            largest = area;
        }             
    }
}

Console.WriteLine(largest);

return;

ulong CalculateArea(Vector2 a, Vector2 b)
{
    return ((ulong)Math.Abs(a.X - b.X) + 1) * ((ulong)Math.Abs(a.Y - b.Y) + 1);
}