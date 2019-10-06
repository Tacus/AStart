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
	public static extern IntPtr AStartAlgWithPath (int startx,int starty,int endx,int endy,int with,int height,out int time,out int size);
	[DllImport (libname)]
	public static extern IntPtr DFSAlgWithPath (int startx,int starty,int endx,int endy,int with,int height,out int time,out int size);

	[DllImport (libname)]
	public static extern IntPtr DFSAlgWithMinPath (int startx,int starty,int endx,int endy,int with,int height,out int time,out int size);

	[DllImport (libname)]
	public static extern IntPtr BFSAlgWithPath (int startx,int starty,int endx,int endy,int with,int height,out int time,out int size);


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
	public void OnGUI()
	{

		int startx = (int)start.x, starty = (int)start.y, endx = (int)end.x, endy = (int)end.y;
		int time, size;
		if (GUI.Button (new Rect (30,200, 200, 100), astartvalue)) {
			IntPtr ptr = AStartAlgWithPath (startx, starty, endx, endy, width, height,out time,out size);
			astartvalue = string.Format ("A* time:{0},path step:{1}" ,time,size);
			pathString = getPath(ptr,size);
		}

		if (GUI.Button (new Rect (400, 200, 200, 100), dfsvalue)) {
			IntPtr ptr = DFSAlgWithPath (startx, starty, endx, endy, width, height,out time,out size);
			dfsvalue = string.Format ("DFS time:{0},path step:{1}" ,time,size);
			pathString = getPath(ptr,size);
		}

		if (GUI.Button (new Rect (400, 400, 200, 100), dfsMinvalue)) {
			IntPtr ptr = DFSAlgWithMinPath (startx, starty, endx, endy, width, height,out time,out size);
			dfsMinvalue = string.Format ("DFS(Min) time:{0},path step:{1}" ,time,size);
			pathString = getPath(ptr,size);
		}

		if (GUI.Button (new Rect (800, 200, 200, 100), bfsvalue)) {
			IntPtr ptr = BFSAlgWithPath (startx, starty, endx, endy, width, height,out time,out size);
			bfsvalue = string.Format ("BFS time:{0},path step:{1}" ,time,size);
			pathString = getPath(ptr,size);
		}

		float cx = 0;
		float cy = (float)(Screen.height*0.6);
		float textHeight = (float)(Screen.height * 0.4);
		Rect mid = new Rect(cx,cy,Screen.width,textHeight);
//		var style = new GUIStyle ();
//		style.fontSize = 20;
//		style.stretchHeight = true;
//		style.stretchWidth = true;
		GUI.TextArea(mid,pathString);

//		if (GUI.Button (new Rect (400, 100, 200, 100), testvalue)) {
//			string time = AVPGetVersion  ();
//			testvalue = string.Format ("AVPGetVersion:{0}" , time);
//		}
	}
	void Start()
	{

	}
//
//	public static int decompressBuffer (byte[] source, int length, ref byte[] outBuffer, int bufferOffset)
//	{
//		GCHandle sbuf = GCHandle.Alloc (source, GCHandleType.Pinned);
//		IntPtr ptr;
//		int siz = 0;
//
//		ptr = zipDecompressBuffer (sbuf.AddrOfPinnedObject (), length, ref siz);
//
//		if (siz == 0 || ptr == IntPtr.Zero) {
//			sbuf.Free ();
//			releaseBuffer (ptr);
//			return -1;
//		}
//
//		var newBufferSize = siz;
//		if (0 < bufferOffset) {
//			newBufferSize += bufferOffset;
//		}
//		if (outBuffer.Length < newBufferSize) {
//			var temp = new byte[newBufferSize];
//			for (int i = 0; i < bufferOffset; ++i) {
//				temp [i] = outBuffer [i];
//			}
//			outBuffer = temp;
//		}
//
//		Marshal.Copy (ptr, outBuffer, 0, siz);
//
//		sbuf.Free ();
//		releaseBuffer (ptr);
//
//		return siz;
//	}
}
