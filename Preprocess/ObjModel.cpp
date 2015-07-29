#include "ObjModel.h"


void ObjModel::PreReadObjModel(
	const std::string filename,
	const std::string outname,
	bool bComputeNormals ,
	bool bAverageNormals,
	bool bComputeTangents,
	bool bAverageTangents
	)
{
	std::wifstream fileIn(filename.c_str());	//Open file

	//Arrays to store our model's information
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;

	//Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;
	bool hasNorm = false;

	//Temp variables to store into vectors
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t checkChar;		//The variable we will use to store one char from file at a time
	std::wstring face;		//Holds the string containing our face vertices
	int vIndex = 0;			//Keep track of our vertex index count
	int triangleCount = 0;	//Total Triangles
	int totalVerts = 0;
	int meshTriangles = 0;

	if (fileIn)
	{
		std::cout << "Reading model data, go grab some drink..." << std::endl;
		while (fileIn)
		{
			checkChar = fileIn.get();

			switch (checkChar)
			{
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;

					vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f));
				}
				if (checkChar == 't')
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;

					vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));

					hasTexCoord = true;
				}
				if (checkChar == 'n')
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;

					vertNorm.push_back(XMFLOAT3(vnx, vny, -vnz));

					hasNorm = true;
				}
				break;
			case 'f':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;
					triangleCount = 0;

					checkChar = fileIn.get();
					while (checkChar != '\n')
					{
						face += checkChar;
						checkChar = fileIn.get();
						if (checkChar == ' ')
							triangleCount++;
					}

					if (face[face.length() - 1] == ' ')
						triangleCount--;

					triangleCount -= 1;

					std::wstringstream ss(face);

					if (face.length() > 0)
					{
						int firstVIndex, lastVIndex;	//Holds the first and last vertice's index

						for (int i = 0; i < 3; ++i)		//First three vertices (first triangle)
						{
							ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm)

							std::wstring vertPart;
							int whichPart = 0;		//(vPos, vTexCoord, or vNorm)

							//Parse this string
							for (int j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								//If the current char is a divider "/", or its the last character in the string
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);	//Used to convert wstring to int

									if (whichPart == 0)	//If vPos
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

										//Check to see if the vert pos was the only thing specified
										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}
									else if (whichPart == 1)	//If vTexCoord
									{
										if (vertPart != L"")	//Check to see if there even is a tex coord
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
										}
										else	//If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										//If the cur. char is the second to last in the string, then
										//there must be no normal, so set a default normal
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;

									}
									else if (whichPart == 2)	//If vNorm
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = L"";	//Get ready for next vertex part
									whichPart++;	//Move on to next vertex part					
								}
							}
							vertPosIndex.push_back(vertPosIndexTemp);
							vertTCIndex.push_back(vertTCIndexTemp);
							vertNormIndex.push_back(vertNormIndexTemp);
							totalVerts++;
							indices.push_back(totalVerts - 1);

							if (i == 0)
							{
								firstVIndex = indices[vIndex];	//The first vertex index of this FACE
							}
							if (i == 2)
							{
								lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE
							}
							vIndex++;	//Increment index count
						}

						meshTriangles++;	//One triangle down

						for (int l = 0; l < triangleCount - 1; ++l)
						{
							indices.push_back(firstVIndex);
							vIndex++;

							indices.push_back(lastVIndex);
							vIndex++;

							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							for (int j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/')
									vertPart += VertDef[j];
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);

									if (whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										if (j == VertDef.length() - 1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;

									}
									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}
							vertPosIndex.push_back(vertPosIndexTemp);
							vertTCIndex.push_back(vertTCIndexTemp);
							vertNormIndex.push_back(vertNormIndexTemp);
							totalVerts++;
							indices.push_back(totalVerts - 1);

							lastVIndex = indices[vIndex];

							meshTriangles++;
							vIndex++;
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		std::cout << "Failed to open model file!" << std::endl;
		return;
	}

	if (!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if (!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));

	fileIn.close();

	std::vector<Vertex::VertexPNTTan> vertices;
	Vertex::VertexPNTTan tempVert;

	for (int j = 0; j < totalVerts; ++j)
	{
		tempVert.Pos = vertPos[vertPosIndex[j]];
		tempVert.Normal = vertNorm[vertNormIndex[j]];
		tempVert.Tex = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
	}

	if (bComputeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		for (int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,1)
			vecX = vertices[indices[(i * 3) + 1]].Pos.x - vertices[indices[(i * 3)]].Pos.x;
			vecY = vertices[indices[(i * 3) + 1]].Pos.y - vertices[indices[(i * 3)]].Pos.y;
			vecZ = vertices[indices[(i * 3) + 1]].Pos.z - vertices[indices[(i * 3)]].Pos.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i * 3) + 2]].Pos.x - vertices[indices[(i * 3)]].Pos.x;
			vecY = vertices[indices[(i * 3) + 2]].Pos.y - vertices[indices[(i * 3)]].Pos.y;
			vecZ = vertices[indices[(i * 3) + 2]].Pos.z - vertices[indices[(i * 3)]].Pos.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

			//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
			tempNormal.push_back(unnormalized);			//Save unormalized normal (for normal averaging)
		}

		if (bAverageNormals)
		{
			//Compute vertex normals (normal Averaging)
			XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			int facesUsing = 0;
			float tX;
			float tY;
			float tZ;

			//Go through each vertex
			for (int i = 0; i < totalVerts; ++i)
			{
				//Check which triangles use this vertex
				for (int j = 0; j < meshTriangles; ++j)
				{
					if (indices[j * 3] == i ||
						indices[(j * 3) + 1] == i ||
						indices[(j * 3) + 2] == i)
					{
						tX = XMVectorGetX(normalSum) + tempNormal[j].x;
						tY = XMVectorGetY(normalSum) + tempNormal[j].y;
						tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

						normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum
						facesUsing++;
					}
				}
				assert(facesUsing > 0);

				normalSum = normalSum / facesUsing;
				normalSum = XMVector3Normalize(normalSum);

				//Store the normal in our current vertex
				vertices[i].Normal.x = XMVectorGetX(normalSum);
				vertices[i].Normal.y = XMVectorGetY(normalSum);
				vertices[i].Normal.z = XMVectorGetZ(normalSum);

				//Clear normalSum and facesUsing for next vertex
				normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				facesUsing = 0;
			}
		}
		else
		{
			for (int i = 0; i < totalVerts; ++i)
			{
				for (int j = 0; j < meshTriangles; ++j)
				{
					if (indices[j * 3] == i ||
						indices[(j * 3) + 1] == i ||
						indices[(j * 3) + 2] == i)
					{
						XMVECTOR normal = XMVectorSet(tempNormal[j].x, tempNormal[j].y, tempNormal[j].z, 0.0f);
						normal = XMVector3Normalize(normal);
						vertices[i].Normal.x = XMVectorGetX(normal);
						vertices[i].Normal.y = XMVectorGetY(normal);
						vertices[i].Normal.z = XMVectorGetZ(normal);
						break;
					}
				}
			}
		}
	}

	if (bComputeTangents)
	{
		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		std::vector<XMFLOAT3> tempTangent;
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float tanU1, tanU2, tanV1, tanV2;
		float vecX, vecY, vecZ;

		for (int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,1)
			vecX = vertices[indices[(i * 3) + 1]].Pos.x - vertices[indices[(i * 3)]].Pos.x;
			vecY = vertices[indices[(i * 3) + 1]].Pos.y - vertices[indices[(i * 3)]].Pos.y;
			vecZ = vertices[indices[(i * 3) + 1]].Pos.z - vertices[indices[(i * 3)]].Pos.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i * 3) + 2]].Pos.x - vertices[indices[(i * 3)]].Pos.x;
			vecY = vertices[indices[(i * 3) + 2]].Pos.y - vertices[indices[(i * 3)]].Pos.y;
			vecZ = vertices[indices[(i * 3) + 2]].Pos.z - vertices[indices[(i * 3)]].Pos.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

			//Find first texture coordinate edge 2d vector
			tanU1 = vertices[indices[(i * 3) + 1]].Tex.x - vertices[indices[(i * 3)]].Tex.x;
			tanV1 = vertices[indices[(i * 3) + 1]].Tex.y - vertices[indices[(i * 3)]].Tex.y;

			//Find second texture coordinate edge 2d vector
			tanU2 = vertices[indices[(i * 3) + 2]].Tex.x - vertices[indices[(i * 3)]].Tex.x;
			tanV2 = vertices[indices[(i * 3) + 2]].Tex.y - vertices[indices[(i * 3)]].Tex.y;

			//assert(!(tanU1 * tanV2 - tanU2 * tanV1 < FLT_MIN &&  tanU1 * tanV2 - tanU2 * tanV1 > -FLT_MIN));

			if (tanU1 * tanV2 - tanU2 * tanV1 < FLT_MIN &&  tanU1 * tanV2 - tanU2 * tanV1 > -FLT_MIN)
				tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			else
			{
				tangent.x = (tanV2 * XMVectorGetX(edge1) - tanV1 * XMVectorGetX(edge2)) * (1.0f / (tanU1 * tanV2 - tanU2 * tanV1));
				tangent.y = (tanV2 * XMVectorGetY(edge1) - tanV1 * XMVectorGetY(edge2)) * (1.0f / (tanU1 * tanV2 - tanU2 * tanV1));
				tangent.z = (tanV2 * XMVectorGetZ(edge1) - tanV1 * XMVectorGetZ(edge2)) * (1.0f / (tanU1 * tanV2 - tanU2 * tanV1));
			}
			tempTangent.push_back(tangent);
		}

		if (bAverageTangents)
		{
			XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			int facesUsing = 0;
			float tX;
			float tY;
			float tZ;

			for (int i = 0; i < totalVerts; ++i)
			{
				for (int j = 0; j < meshTriangles; ++j)
				{
					if (indices[j * 3] == i ||
						indices[(j * 3) + 1] == i ||
						indices[(j * 3) + 2] == i)
					{
						tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
						tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
						tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

						tangentSum = XMVectorSet(tX, tY, tZ, 0.0f);
						facesUsing++;
					}
				}
				assert(facesUsing > 0);
				tangentSum = tangentSum / facesUsing;
				tangentSum = XMVector3Normalize(tangentSum);

				vertices[i].Tan.x = XMVectorGetX(tangentSum);
				vertices[i].Tan.y = XMVectorGetY(tangentSum);
				vertices[i].Tan.z = XMVectorGetZ(tangentSum);

				tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				facesUsing = 0;
			}
		}
		else
		{
			for (int i = 0; i < totalVerts; ++i)
			{
				for (int j = 0; j < meshTriangles; ++j)
				{
					if (indices[j * 3] == i ||
						indices[(j * 3) + 1] == i ||
						indices[(j * 3) + 2] == i)
					{
						XMVECTOR tangent = XMVectorSet(tempTangent[j].x, tempTangent[j].y, tempTangent[j].z, 0.0f);
						tangent = XMVector3Normalize(tangent);
						vertices[i].Tan.x = XMVectorGetX(tangent);
						vertices[i].Tan.y = XMVectorGetY(tangent);
						vertices[i].Tan.z = XMVectorGetZ(tangent);
						break;
					}
				}
			}
		}
	}

	std::cout <<"Outputing data, just a  second..."<< std::endl;
	FILE * fout = fopen(outname.c_str(), "w");

	fprintf(fout, "%d\n", totalVerts);
	for (int i = 0; i < totalVerts; i++)
	{
		Vertex::VertexPNTTan v = vertices[i];
		fprintf(fout, "%f %f %f %f %f %f %f %f %f %f %f\n", v.Pos.x, v.Pos.y, v.Pos.z, v.Normal.x, v.Normal.y, v.Normal.z, v.Tex.x, v.Tex.y, v.Tan.x, v.Tan.y, v.Tan.z);
	}
	
	fprintf(fout, "%d\n", meshTriangles);
	for (int i = 0; i < meshTriangles; i++)
	{
		fprintf(fout, "%d %d %d\n", indices[i *3], indices[i*3+1], indices[i*3+2]); 
	}

	fclose(fout);

	std::cout << "Done!" << std::endl;

	return;
}

ObjModel::ObjModel()
{
}


ObjModel::~ObjModel()
{
}
