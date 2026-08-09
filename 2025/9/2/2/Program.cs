using _2;
using Raylib_cs;


var input = File.ReadAllLines("../../../input.txt");

var points = new Vector2I[input.Length];

for (var i = 0; i < input.Length; i++)
{
    var coords = input[i].Split(',');
    points[i] = new Vector2I(int.Parse(coords[0]), int.Parse(coords[1]));
}

ulong largest = 0;
var vecA = new Vector2I(0, 0);
var vecB = new Vector2I(0, 0);
var interceptions = 0;

for (var i = 0; i < points.Length - 1; i++)
{
    for (var j = i + 1; j < points.Length; j++)
    {
        var area = CalculateArea(points[i], points[j]);
        var interception = HasInterceptions(points[i], points[j], points);
        if (interception)
        {
            interceptions++;
        }
        
        if (area > largest && !interception)
        {
            largest = area;
            vecA = points[i];
            vecB = points[j];
        }             
    }
}

Console.WriteLine(largest);
//Console.WriteLine($"Interceptions: {interceptions}");

/*Raylib.InitWindow(1000, 1000, "C# Graphics Window");
Raylib.SetTargetFPS(60);

while (!Raylib.WindowShouldClose())
{
    Raylib.BeginDrawing();
    Raylib.ClearBackground(Color.DarkGray);
    
    for (var i = 0; i < points.Length - 1; i++)
    {
        Raylib.DrawLine(points[i].X / 100, points[i].Y / 100, points[i + 1].X / 100, points[i + 1].Y / 100, Color.Red);   
    }
    
    Raylib.DrawRectangleLines(vecA.X / 100, vecA.Y / 100, (vecB.X - vecA.X) / 100, (vecB.Y - vecA.Y) / 100, Color.Green);
    //Raylib.DrawRectangle(vecA.X > vecB.X ? vecB.X : vecA.X, vecA.Y > vecB.Y ? vecB.Y : vecA.Y, Math.Abs(vecB.X - vecA.X), Math.Abs(vecB.Y - vecA.Y), Color.Green);


    Raylib.DrawLine(points[0].X / 100, points[0].Y / 100, points[^1].X / 100, points[^1].Y / 100, Color.Red);  
    
    Raylib.EndDrawing();
}*/

return;

ulong CalculateArea(Vector2I a, Vector2I b)
{
    return ((ulong)Math.Abs(a.X - b.X) + 1) * ((ulong)Math.Abs(a.Y - b.Y) + 1);
}

bool HasInterceptions(Vector2I a, Vector2I b, Vector2I[] arr)
{
    var lineA = new OrtLine(a, new Vector2I(a.X, b.Y));
    var lineB = new OrtLine(a, new Vector2I(b.X, a.Y));
    var lineC = new OrtLine(b, new Vector2I(a.X, b.Y));
    var lineD = new OrtLine(b, new Vector2I(b.X, a.Y));
    var lineE = new OrtLine(new Vector2I(a.X + (b.X - a.X) / 2, a.Y), new Vector2I(a.X + (b.X - a.X) / 2, b.Y));
    var lineF = new OrtLine(new Vector2I(a.X, a.Y + (b.Y - a.Y) / 2), new Vector2I(b.X, a.Y + (b.Y - a.Y) / 2));
    
    for (var i = 0; i < arr.Length; i++)
    {
        OrtLine line;
        if (i == arr.Length - 1)
        {
            line = new OrtLine(arr[i], arr[0]);
        }
        else
        {
            line = new OrtLine(arr[i], arr[i + 1]);
        }

        if (lineA.IsCrossing(line) || lineB.IsCrossing(line) || lineC.IsCrossing(line) || lineD.IsCrossing(line) || lineE.IsCrossing(line) || lineF.IsCrossing(line)) return true;
    }
    
    return false;
}



