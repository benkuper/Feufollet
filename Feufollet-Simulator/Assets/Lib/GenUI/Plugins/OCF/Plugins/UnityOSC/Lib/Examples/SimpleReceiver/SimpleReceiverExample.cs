using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityOSC;

public class SimpleReceiverExample : MonoBehaviour {
	
	private OSCReceiver receiver;

	public int port = 8338;
	
	// Use this for initialization
	void Start () {
		receiver = new OSCReceiver();
		receiver.Open(port);
        receiver.messageReceived += HandleMessage;
	}
	
    void HandleMessage(OSCMessage msg)
    {
        Debug.Log(string.Format("message received: {0} {1}", msg.Address, DataToString(msg.Data)));
    }

	private string DataToString(List<object> data)
	{
		string buffer = "";
		
		for(int i = 0; i < data.Count; i++)
		{
			buffer += data[i].ToString() + " ";
		}
		
		buffer += "\n";
		
		return buffer;
	}
}
