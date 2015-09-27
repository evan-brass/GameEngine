#pragma once

enum RenderOperationValidation
{
	RenderOperationValidation_ShadersLoaded   = 1 << 0,
	RenderOperationValidation_ShadersCompiled = 1 << 2,
	RenderOperationValidation_ProgramLinked   = 1 << 3,
	RenderOperationValidation_VerticesLoaded  = 1 << 4,
	RenderOperationValidation_DataBuffered    = 1 << 5,
	RenderOperationValidation_Valid           = RenderOperationValidation_ShadersLoaded |
												RenderOperationValidation_ShadersCompiled |
												RenderOperationValidation_ProgramLinked |
												RenderOperationValidation_VerticesLoaded |
												RenderOperationValidation_DataBuffered,
};
enum RenderOperationDrawType
{
	RenderOperationDrawType_Arrays        = 1 << 0,
};
enum RenderOperationShaders
{
	RenderOperationShader_Geometry = 1 << 0,
	RenderOperationShader_Vertex   = 1 << 1,
	RenderOperationShader_Fragment = 1 << 2,
};

class RenderOperation
{
public:
	// Linked list
	RenderOperation * head;
	RenderOperation * tail;
	RenderOperation * next;
	RenderOperation * prev;

	RenderOperationValidation validation;
	RenderOperationShaders shaders;

	RenderOperation();

	// setup
	bool useShader(char * shader);
	bool useProgram(char * program);

	// each render
	bool validate();
	void render();

	virtual ~RenderOperation();

private:
	bool loadShaders();
	bool compileShaders();
	bool linkProgram();

};

