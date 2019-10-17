using System;
using UnityEngine;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
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
	public static extern int AStartAlgWithPath (byte[]worldMap,uint[]pathMap, int startx,int starty,int endx,int endy,int with,int height,out int time);
	[DllImport (libname)]
	public static extern int DFSAlgWithPath (byte[]worldMap,uint[]pathMap, int startx,int starty,int endx,int endy,int with,int height,out int time);

	[DllImport (libname)]
	public static extern int DFSAlgWithMinPath (byte[]worldMap,uint[]pathMap, int startx,int starty,int endx,int endy,int with,int height,out int time);

	[DllImport (libname)]
	public static extern int BFSAlgWithPath (byte[]worldMap,uint[]pathMap, int startx,int starty,int endx,int endy,int with,int height,out int time);


	[DllImport (libname)]
	public static extern void releaseBuffer (IntPtr ptr);
//	[DllImport (libname)]
//	public static extern float FooPluginFunction ();
	#endif
	#if ((UNITY_IOS  || UNITY_IPHONE) &&(!UNITY_EDITOR))
	[DllImport("__Internal")]
	public static extern Int64 AStartAlg(int startx,int starty,int endx,int endy,int with,int height);
	[DllImport("__Internal")]
	public static extern Int64 DFSAlg(int startx,int starty,int endx,int endy,int with,int height);
	[DllImport("__Internal")]
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
	private string dfsvalue = string.Format("DFS");
	private string dfsMinvalue = string.Format("DFS(Min)");
	private string bfsvalue = string.Format("BFS");
	private string pathString;

	private string getPath(IntPtr ptr,int size)
	{
		int len = size * 8;
		byte[] paths = new byte[len];
		StringBuilder sb = new StringBuilder ();
		Marshal.Copy (ptr, paths, 0, len);
		int offset;
		for (int i = 0; i < size; i++) {
			offset = i * 8;
			uint x = BitConverter.ToUInt32 (paths, offset);
			uint y = BitConverter.ToUInt32 (paths, offset+4);
			sb.Append(string.Format("({0},{1})=>",x,y));
		}
		releaseBuffer (ptr);
		return sb.ToString ();
	}

	private string getPath(uint[]pathMap,int size)
	{
		StringBuilder sb = new StringBuilder ();
		int offset;
		for (int i = 0; i < size; i++) {
			offset = i * 2;
			uint x = pathMap[offset];
			uint y = pathMap[offset+1];
			sb.Append(string.Format("({0},{1})=>",x,y));
		}
		return sb.ToString ();
	}

	public static void initRelsult()
	{
		
	}

	public static byte[] getWorldMap(int width,int height)
	{
		int size = width * height;
		byte[] worldMap = new byte[size];
		for (int i = 0; i < size; i++) {
			worldMap [i] = 1;
		}

		return worldMap;
	}

	public void OnGUI()
	{

		int startx = (int)start.x, starty = (int)start.y, endx = (int)end.x, endy = (int)end.y;
		int time, size;
		if (GUI.Button (new Rect (30,200, 200, 100), astartvalue)) {
			byte[] worldMap = getWorldMap (width,height);
			uint[] pathMap = new uint[width * height];
			int pathStep = AStartAlgWithPath (worldMap,pathMap,startx, starty, endx, endy, width, height,out time);
			astartvalue = string.Format ("A* time:{0},path step:{1}" ,time,pathStep);
			pathString = getPath(pathMap,pathStep);
		}

		if (GUI.Button (new Rect (400, 200, 200, 100), dfsvalue)) {
			byte[] worldMap = getWorldMap (width,height);
			uint[] pathMap = new uint[width * height];
			int pathStep = DFSAlgWithPath (worldMap,pathMap,startx, starty, endx, endy, width, height,out time);
			dfsvalue = string.Format ("DFS time:{0},path step:{1}" ,time,pathStep);
			pathString = getPath(pathMap,pathStep);
		}

		if (GUI.Button (new Rect (400, 400, 200, 100), dfsMinvalue)) {
			byte[] worldMap = getWorldMap (width,height);
			uint[] pathMap = new uint[width * height];
			int pathStep = DFSAlgWithMinPath (worldMap,pathMap,startx, starty, endx, endy, width, height,out time);
			dfsMinvalue = string.Format ("DFS(Min) time:{0},path step:{1}" ,time,pathStep);
			pathString = getPath(pathMap,pathStep);
		}

		if (GUI.Button (new Rect (800, 200, 200, 100), bfsvalue)) {
			byte[] worldMap = getWorldMap (width,height);
			uint[] pathMap = new uint[width * height];
			int pathStep = BFSAlgWithPath (worldMap,pathMap,startx, starty, endx, endy, width, height,out time);
			bfsvalue = string.Format ("BFS time:{0},path step:{1}" ,time,pathStep);
			pathString = getPath(pathMap,pathStep);
		}

		float cx = 0;
		float cy = (float)(Screen.height*0.6);
		float textHeight = (float)(Screen.height * 0.4);
		Rect mid = new Rect(cx,cy,Screen.width,textHeight);
		GUI.TextArea(mid,pathString);
	}
	void Start()
	{

	}

}
