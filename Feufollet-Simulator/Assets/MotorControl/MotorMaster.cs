using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityOSC;

[ExecuteInEditMode]
public class MotorMaster : MonoBehaviour
{
    Motor[] motors;
    MotorTarget target;

    public bool sendTargetPosition;
    public bool sendMotorDistances;

    bool validPosition;
    float radius;

    void Awake()
    {
        motors = GetComponentsInChildren<Motor>();
        target = GetComponentInChildren<MotorTarget>();
    }

    void Update()
    {

        if (motors.Length < 2) return;
        float maxY = 100;
        float minX = 0;
        float maxX = 0;

        int index = 0;
        validPosition = true;
        foreach (Motor m in motors)
        {
            maxY = Mathf.Min(m.transform.localPosition.y, maxY);
            minX = Mathf.Min(m.transform.localPosition.x, minX);
            maxX = Mathf.Max(m.transform.localPosition.x, maxX);

            Vector2 targetPos = (Vector2)target.transform.localPosition + target.targetOffsets[index];
            m.position = Vector2.Distance(targetPos, m.transform.localPosition);

            if (m.position < 0 || targetPos.y < 0 || targetPos.y >= m.transform.localPosition.y
                || (index == 0 && targetPos.x < m.transform.localPosition.x)
                || (index == 1 && targetPos.x > m.transform.localPosition.x)
                )
            {
                validPosition = false;
            }

            index++;
        }

        if(validPosition)
        {
            sendPositions();
        }
    }


    void sendPositions()
    {
        if(sendTargetPosition)
        {
            OSCMessage m = new OSCMessage("/motor/targetPosition");
            m.Append(target.transform.localPosition.x);
            m.Append(target.transform.localPosition.y);
            OSCMaster.SendMessage(m, "127.0.0.1", 14000);
        }

        if(sendMotorDistances)
        {
            for (int i = 0; i < motors.Length; i++)
            {

                OSCMessage m = new OSCMessage("/motor/" + (i + 1) + "/distance");
                m.Append(motors[i].position);
                OSCMaster.SendMessage(m, "127.0.0.1", 14000);
            }
        }
      
    }

    private void OnDrawGizmos()
    {
        if (motors == null) return;
        if (motors.Length < 2) return;
       
        Gizmos.color = validPosition ? Color.green : Color.red;

        int index = 0;
        foreach(Motor m in motors)
        {
            Vector2 v = (target.transform.position + (Vector3)target.targetOffsets[index] - m.transform.position).normalized * m.position;
            Gizmos.DrawLine((Vector2)m.transform.position, (Vector2)m.transform.position + v);
            index++;
        }
    }

}
