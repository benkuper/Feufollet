using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TrigoUtil : MonoBehaviour
{
    // Find the points where the two circles intersect.
    static public int FindCircleCircleIntersections(
        Vector2 center0, float radius0,
        Vector2 center1, float radius1,
        out Vector2 intersection1, out Vector2 intersection2)
    {
        // Find the distance between the centers.
        float dx = center0.x - center1.x;
        float dy = center0.y - center1.y;
        double dist = Mathf.Sqrt(dx * dx + dy * dy);

        // See how many solutions there are.
        if (dist > radius0 + radius1)
        {
            // No solutions, the circles are too far apart.
            intersection1 = new Vector2(float.NaN, float.NaN);
            intersection2 = new Vector2(float.NaN, float.NaN);
            return 0;
        }
        else if (dist < Mathf.Abs(radius0 - radius1))
        {
            // No solutions, one circle contains the other.
            intersection1 = new Vector2(float.NaN, float.NaN);
            intersection2 = new Vector2(float.NaN, float.NaN);
            return 0;
        }
        else if ((dist == 0) && (radius0 == radius1))
        {
            // No solutions, the circles coincide.
            intersection1 = new Vector2(float.NaN, float.NaN);
            intersection2 = new Vector2(float.NaN, float.NaN);
            return 0;
        }
        else
        {
            // Find a and h.
            double a = (radius0 * radius0 -
                radius1 * radius1 + dist * dist) / (2 * dist);
            double h = Mathf.Sqrt((float)(radius0 * radius0 - a * a));

            // Find P2.
            double cx2 = center0.x + a * (center1.x - center0.x) / dist;
            double cy2 = center0.y + a * (center1.y - center0.y) / dist;

            // Get the points P3.
            intersection1 = new Vector2(
                (float)(cx2 + h * (center1.y - center0.y) / dist),
                (float)(cy2 - h * (center1.x - center0.x) / dist));
            intersection2 = new Vector2(
                (float)(cx2 - h * (center1.y - center0.y) / dist),
                (float)(cy2 + h * (center1.x - center0.x) / dist));

            // See if we have 1 or 2 solutions.
            if (dist == radius0 + radius1) return 1;
            return 2;
        }
    }
}
