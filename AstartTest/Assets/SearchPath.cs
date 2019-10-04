using System;
using UnityEngine;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;
using System.IO.Compression;
public class SearchPath : MonoBehaviour
{
	#if ( UNITY_ANDROID || UNITY_STANDALONE_LINUX || UNITY_EDITOR || UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX)
	#if (UNITY_ANDROID || UNITY_STANDALONE_LINUX)
	private const string libname = "zgb";
	#elif(UNITY_EDITOR || UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX )
	private const string libname = "libzgb";
	#endif
	[DllImport (libname)]
	public static extern Int64 AStartAlg (int startx,int starty,int endx,int endy,int with,int height);
	[DllImport (libname)]
	public static extern Int64 DFSAlg (int startx,int starty,int endx,int endy,int with,int height);
	[DllImport (libname)]
	public static extern Int64 BFSAlg (int startx,int starty,int endx,int endy,int with,int height);

//	[DllImport (libname)]
//	public static extern float FooPluginFunction ();
	#endif
	#if ((UNITY_IOS  || UNITY_IPHONE) &&(!UNITY_EDITOR))
	[DllImport("__Internal")]
	public static extern Int64 AStartAlg(int startx,int starty,int endx,int endy,int with,int height);
	public static extern Int64 DFSAlg(int startx,int starty,int endx,int endy,int with,int height);
	public static extern Int64 BFSAlg(int startx,int starty,int endx,int endy,int with,int height);
	#endif

//	#if UNITY_EDITOR_OSX
//	[DllImport("AVProVideo")]
//	public static extern string AVPGetVersion();
//	#else
//	public static  string AVPGetVersion(){ return "";};
//
//	#endif
//
	public int width;
	public int height;
	public Vector2 start;
	public Vector2 end;
	private string astartvalue = string.Format("A*");
	private string dfsvalue = string.Format("dfs");
	private string bfsvalue = string.Format("bfs");
	private string testvalue = string.Format("test");
	public void OnGUI()
	{
		
		int startx = (int)start.x, starty = (int)start.y, endx = (int)end.x, endy = (int)end.y;
		if (GUI.Button (new Rect (30,300, 200, 100), astartvalue)) {
			Int64 time = AStartAlg (startx, starty, endx, endy, width, height);
			astartvalue = string.Format ("A* time:{0}" ,time);
		}

		if (GUI.Button (new Rect (400, 300, 200, 100), dfsvalue)) {
			Int64 time = DFSAlg (startx, starty, endx, endy, width, height);
			dfsvalue = string.Format ("DFS time:{0}" , time);
		}

		if (GUI.Button (new Rect (800, 300, 200, 100), bfsvalue)) {
			Int64 time = BFSAlg  (startx, starty, endx, endy, width, height);
			bfsvalue = string.Format ("BFS time:{0}" ,time);
		}

//		if (GUI.Button (new Rect (400, 100, 200, 100), testvalue)) {
//			string time = AVPGetVersion  ();
//			testvalue = string.Format ("AVPGetVersion:{0}" , time);
//		}
	}
}
