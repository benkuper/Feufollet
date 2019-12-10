using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class MotorTarget : MonoBehaviour
{
    [Range(0, 1)]
    public float offset;
    public Vector2[] targetOffsets { get; set; }

    void Start()
    {
        targetOffsets = new Vector2[2];    
    }

    void Update()
    {
        if (targetOffsets.Length < 2) return;

        float angle = offset * Mathf.PI / 2;
        float tAngle1 = Mathf.PI / 2 + angle;
        float tAngle2 = Mathf.PI / 2 - angle;

        targetOffsets[0] = new Vector2(Mathf.Cos(tAngle1), Mathf.Sin(tAngle1)) * transform.localScale.x / 2;
        targetOffsets[1] = new Vector2(Mathf.Cos(tAngle2), Mathf.Sin(tAngle2)) * transform.localScale.x / 2;
    }
}
