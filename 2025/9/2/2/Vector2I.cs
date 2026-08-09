using System.Numerics;

namespace _2;

public record Vector2I(int X, int Y);

public record OrtLine
{
    public Vector2I A { get; }
    public Vector2I B { get; }
    public bool Horizontal { get; }

    public OrtLine(Vector2I a, Vector2I b)
    {
        Horizontal = a.X != b.X;

        if (Horizontal)
        {
            if (a.X < b.X)
            {
                A = a;
                B = b;
            }
            else
            {
                A = b;
                B = a;
            }
        }
        else
        {
            if (a.Y < b.Y)
            {
                A = a;
                B = b;
            }
            else
            {
                A = b;
                B = a;
            }
        }
    }
    
    public bool IsCrossing(OrtLine other)
    {
        if (Horizontal == other.Horizontal) return false;

        if (Horizontal && A.X < other.A.X && B.X > other.A.X && A.Y > other.A.Y && A.Y < other.B.Y) return true;
        
        if (!Horizontal && A.Y < other.A.Y && B.Y > other.A.Y && A.X > other.A.X && A.X < other.B.X) return true;
        
        return false;
    }
} 