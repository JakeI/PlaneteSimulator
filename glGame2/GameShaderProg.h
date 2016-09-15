#pragma once
#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"
class GameShaderProg :
	public ShaderProg
{
private:
	GLuint matrixID;
public:
	GameShaderProg() : ShaderProg(){
		matrixID = 0;
	}
	GameShaderProg(char* VertexShaderFilePath, char* FragmentShaderFilePath)
		: ShaderProg(VertexShaderFilePath, FragmentShaderFilePath) {
		LoadMatirxUniformID();
	}
	~GameShaderProg(){}
	void uploadMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
	}
	void LoadMatirxUniformID() {
		matrixID = glGetUniformLocation(ProgrammID, "MVP");
		//MessageBox(NULL, std::to_string(matrixID).c_str(), "MatrixID", MB_OK);
	}
	void draw(GLuint vao, GLuint vboPos, GLuint vboCol, glm::mat4 mvp) {
		use(true);

		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboPos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, vboCol);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		uploadMatrix(mvp);

		glDrawArrays(GL_LINES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		use(false);
	}
};

