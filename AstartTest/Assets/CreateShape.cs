//using UnityEngine;
//using System.Collections;
//
///*仅仅创建Mesh
// *
// *
// */
//public class CreateShape : MonoBehaviour
//{
//
//	private GameObject mMesh;
//	private Material mMaterial;
//
//	/*  Mesh属性
//     *      长宽
//     *      段数
//     *      高度
//     *      高度差
//     */
//	private Vector2 size;//长度和宽度
//	private float minHeight = -10;//最小高度
//	private float maxHeight = 10;//最大高度
//	private Vector2 segment;//长度的段数和宽度的段数
//	private float unitH;//最小高度和最大高度只差，值为正
//
//	/*  顶点属性
//     *      顶点
//     *      uv
//     *      三角形 
//     */
//	private Vector3[] vertexes;//顶点数
//	private Vector2 uvs;//uvs坐标
//	private int[] triangles;//三角形索引
//
//	void Start()
//	{
//		creatMesh(100, 100, 3, 3, -10, 10);
//		
//	}
//
//	private void creatMesh(float width, float height, uint segmentX, uint segmentY, int min, int max)
//	{
//		size = new Vector2(width, height);
//		maxHeight = max;
//		minHeight = min;
//		unitH = maxHeight - minHeight;
//		segment = new Vector2(segmentX, segmentY);
//
//		if (mMesh != null)
//		{
//			Destroy(mMesh);
//		}
//		mMesh = new GameObject();
//		mMesh.name = "CreateMesh";
//
//		computeVertexes();
//		DrawMesh();
//	}
//
//	private void computeVertexes()
//	{
//		int sum = Mathf.FloorToInt((segment.x + 1) * (segment.y + 1));//顶点总数
//		float w = size.x / segment.x;//每一段的长度
//		float h = size.y / segment.y;
//
//		GetTriangles();
//
//		int index = 0;
//		vertexes = new Vector3[sum];
//		for (int i = 0; i < segment.y + 1; i++)
//		{
//			for (int j = 0; j < segment.x + 1; j++)
//			{
//				float tempHeight = 0;
//				vertexes[index] = new Vector3(j * w, 0, i * h);
//				index++;
//			}
//		}
//	}
//
//	private void DrawMesh()
//	{
//		Mesh mesh = mMesh.AddComponent<MeshFilter>().mesh;//网格
//		mMesh.AddComponent<MeshRenderer>();//网格渲染器
//
//		mMaterial = new Material(Shader.Find("Diffuse"));//材质
//
//		mMesh.GetComponent<Renderer>().material = mMaterial;
//
//		/*设置mesh*/
//		mesh.Clear();//更新
//		mesh.vertices = vertexes;
//		//mesh.uv 
//		mesh.triangles = triangles;
//
//		mesh.RecalculateNormals();
//		mesh.RecalculateBounds();
//
//	}
//
//	private int[] GetTriangles()
//	{
//		int sum = Mathf.FloorToInt(segment.x * segment.y * 6);//三角形顶点总数
//		triangles = new int[sum];
//		uint index = 0;
//		for (int i = 0; i < segment.y; i++)
//		{
//			for (int j = 0; j < segment.x; j++)
//			{
//				int role = Mathf.FloorToInt(segment.x) + 1;
//				int self = j + (i * role);
//				int next = j + ((i + 1) * role);
//				//顺时针
//				triangles[index] = self;
//				triangles[index + 1] = next + 1;
//				triangles[index + 2] = self + 1;
//				triangles[index + 3] = self;
//				triangles[index + 4] = next;
//				triangles[index + 5] = next + 1;
//				index += 6;
//			}
//		}
//		return triangles;
//	}
//}

using UnityEngine;
using System.Collections;

public class CreateShape : MonoBehaviour {
	//LineRenderer
	private LineRenderer lineRenderer;
	//定义一个Vector3,用来存储鼠标点击的位置
	private Vector3 position;
	//用来索引端点
	private int index = 0;
	//端点数
	private int LengthOfLineRenderer=0;

	void Start()
	{
		//添加LineRenderer组件
		lineRenderer = gameObject.AddComponent<LineRenderer>();
		//设置材质
		lineRenderer.material = new Material(Shader.Find("Particles/Additive"));
		//设置颜色
		lineRenderer.SetColors(Color.red, Color.yellow);
		//设置宽度
		lineRenderer.SetWidth(0.02f, 0.02f);

	}

	void Update()
	{  
		//获取LineRenderer组件
		lineRenderer = GetComponent<LineRenderer>();
		//鼠标左击
		if (Input.GetMouseButtonDown(0))
		{
			//将鼠标点击的屏幕坐标转换为世界坐标，然后存储到position中
			position = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x,Input.mousePosition.y,1.0f)); 
			//端点数+1
			LengthOfLineRenderer++;
			//设置线段的端点数
			lineRenderer.SetVertexCount(LengthOfLineRenderer);

		}
		//连续绘制线段
		while (index < LengthOfLineRenderer)
		{   
			//两点确定一条直线，所以我们依次绘制点就可以形成线段了
			lineRenderer.SetPosition(index, position);
			index++;
		} 


	}

	void OnGUI()
	{          
		GUILayout.Label("当前鼠标X轴位置：" + Input.mousePosition.x);
		GUILayout.Label("当前鼠标Y轴位置：" + Input.mousePosition.y);        
	}


}