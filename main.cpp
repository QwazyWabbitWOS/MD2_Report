// MD2_Report
// Report the values in the MD2 specified by the file name given.
// Usage: MD2_Report <filename[.md2]>
// If no extension is given the .md2 is automatically added.
//

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	//non-MFC
#include <Windows.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <stdexcept>

//The file header
struct MD2_Header
{
	int Magic;
	int Version;
	int SkinWidth;
	int SkinHeight;
	int FrameSize;
	int NumSkin;
	int NumVertex;
	int NumTexCoords;
	int NumTriangle;
	int NumGlCommand;
	int NumFrame;
	int OffsetSkins;
	int OffsetTexCoords;
	int OffsetTriangles;
	int OffsetFrames;
	int OffsetGlCommands;
	int OffsetEnd;
};

//An skin
struct MD2_Skin
{
	char Name[64];
};

//Texture coords
struct MD2_TexCoord
{
	short U;
	short V;
};

//An triangle
struct MD2_Triangle
{
	unsigned short Verts[3];
	unsigned short Normals[3];
};

//An vertex
struct MD2_Vertex
{
	unsigned char Vector[3];
	unsigned char NormalIndex;
};

//An frame
struct MD2_Frame
{
	float       Scale[3];
	float       Translate[3];
	char        Name[16];
	MD2_Vertex* Verts;
};

//Scaled vertex
struct MD2_VertexS
{
	float Vector[3];
};

//MD2 file to return
struct MD2_File
{
	int           NumVertex;
	int           NumTriangle;
	MD2_Triangle* TriangleData;
	MD2_VertexS* Vertices;
};

bool LoadMD2(char* file, MD2_File** data)
{
	//Vars
	FILE* File = NULL;
	MD2_Header     Header;

	//Open the file
	errno_t status = fopen_s(&File, file, "rb");
	if (status != 0)
	{
		fprintf(stderr, "Can't open file %s\n", file);
		return EXIT_FAILURE;
	}

	memset(&Header, 0, sizeof Header);

	//Read the file header
	fread(&Header, 1, sizeof(MD2_Header), File);

	printf("magic:\t\t\t%d at %zu\n", Header.Magic, offsetof(struct MD2_Header, Magic));
	printf("version:\t\t%d at %zu\n", Header.Version, offsetof(struct MD2_Header, Version));
	printf("skinWidth:\t\t%d at %zu\n", Header.SkinWidth, offsetof(struct MD2_Header, SkinWidth));
	printf("skinHeight:\t\t%d at %zu\n", Header.SkinHeight, offsetof(struct MD2_Header, SkinHeight));
	printf("frameSize:\t\t%d at %zu\n", Header.FrameSize, offsetof(struct MD2_Header, FrameSize));
	printf("numVertices:\t\t%d at %zu\n", Header.NumVertex, offsetof(struct MD2_Header, NumVertex));
	printf("numTexCoords:\t\t%d at %zu\n", Header.NumTexCoords, offsetof(struct MD2_Header, NumTexCoords));
	printf("numTriangles:\t\t%d at %zu\n", Header.NumTriangle, offsetof(struct MD2_Header, NumTriangle));
	printf("numGlCommands:\t\t%d at %zu\n", Header.NumGlCommand, offsetof(struct MD2_Header, NumGlCommand));
	printf("numFrames:\t\t%d at %zu\n", Header.NumFrame, offsetof(struct MD2_Header, NumFrame));
	printf("offsetSkins:\t\t%d at %zu\n", Header.OffsetSkins, offsetof(struct MD2_Header, OffsetSkins));
	printf("offsetTexCoords:\t%d at %zu\n", Header.OffsetTexCoords, offsetof(struct MD2_Header, OffsetTexCoords));
	printf("offsetTriangles:\t%d at %zu\n", Header.OffsetTriangles, offsetof(struct MD2_Header, OffsetTriangles));
	printf("offsetFrames:\t\t%d at %zu\n", Header.OffsetFrames, offsetof(struct MD2_Header, OffsetFrames));
	printf("offsetGlCommands:\t%d at %zu\n", Header.OffsetGlCommands, offsetof(struct MD2_Header, OffsetGlCommands));
	printf("offsetEnd:\t\t%d at %zu\n", Header.OffsetEnd, offsetof(struct MD2_Header, OffsetEnd));

	fclose(File);
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	char filename[MAX_PATH]{ 0 };
	MD2_File** filedata{ 0 };

	/* Check for proper number of command line arguments */
	if (argc < 2)
	{
		fputs("Usage: MD2_Report filename.md2\n\n", stderr);
		return EXIT_FAILURE;
	}

	/* Add the .md2 extension to the file name if no extension exists */
	strncpy_s(filename, argv[1], sizeof filename - 1); /* zero trust! */
	filename[sizeof filename - 1] = '\0';    /* guarantee zero termination */
	if (!strrchr(filename, '.'))
		strcat_s(filename, ".md2");

	fprintf(stdout, "MD2 Header Report for %s\n\n", filename);
	return LoadMD2(filename, filedata);
}