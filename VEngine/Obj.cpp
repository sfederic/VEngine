#include "Obj.h"
#include <Windows.h>

bool LoadOBJFile(const char* filename, ModelData& data)
{
	std::vector<XMFLOAT3> v; //Vertex posisitons
	std::vector<XMFLOAT2> vt; //Texcoords
	std::vector<XMFLOAT3> vn; //Normals
	std::vector<unsigned int> vf; //Faces
	std::vector<unsigned int> vtf;
	std::vector<unsigned int> vnf; 

	FILE* file = nullptr;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		char errmsg[512];
		snprintf(errmsg, sizeof(errmsg), "%s not found.", filename);
		MessageBox(0, errmsg, "loadOBJFile", 0);
		return false;
	}

	while (1)
	{
		char line[256];
		int res = fscanf_s(file, "%s", line, _countof(line));

		if (res == EOF) //EOF 
		{
			break;
		}

		if (strcmp(line, "v") == 0) //Vertices
		{
			float v1, v2, v3;
			fscanf_s(file, "%f %f %f\n", &v1, &v2, &v3);
			v.push_back(XMFLOAT3(v1, v2, v3));
		}
		else if (strcmp(line, "vt") == 0) //Texcoords
		{
			float v1, v2;
			fscanf_s(file, "%f %f\n", &v1, &v2);
			vt.push_back(XMFLOAT2(v1, v2));
		}
		else if (strcmp(line, "vn") == 0) //Normals
		{
			float v1, v2, v3;
			fscanf_s(file, "%f %f %f\n", &v1, &v2, &v3);
			vn.push_back(XMFLOAT3(v1, v2, v3));
		}
		else if (strcmp(line, "f") == 0) //Faces
		{
			unsigned int f[9] = {0}; //careful there's no overflow here. OBJ test files are small enough
			fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
				&f[0], &f[1], &f[2],
				&f[3], &f[4], &f[5],
				&f[6], &f[7], &f[8], _countof(f));
			
			vf.push_back(f[0]); vtf.push_back(f[1]); vnf.push_back(f[2]);
			vf.push_back(f[3]); vtf.push_back(f[4]); vnf.push_back(f[5]);
			vf.push_back(f[6]); vtf.push_back(f[7]); vnf.push_back(f[8]);
		}
	}

	int reserveSize = vf.size();
	if (data.verts.size() > 0) { data.verts.reserve(reserveSize); }

	for (int i = 0; i < vf.size(); i++)
	{
		Vertex vertex = {};

		int vf_index = vf[i] - 1;
		assert(vf_index < v.size());
		vertex.pos = v[vf_index];

		int vtf_index = vtf[i] - 1;
		assert(vtf_index < vt.size());
		vertex.uv = vt[vtf_index];

		int vnf_index = vnf[i] - 1;
		assert(vnf_index < vn.size());
		vertex.normal = vn[vnf_index];

		data.verts.push_back(vertex);
	}

	fclose(file);
	return true;
}
