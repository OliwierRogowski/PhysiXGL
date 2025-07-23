#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Shadery
const char* vertexShaderSourceUniform = R"(
#version 330 core
	layout(location = 0) in vec3 aPos;
	uniform vec3 offset;

	void main(){
		gl_Position = vec4(aPos + offset, 1.0);
	}
)";

const char* fragmentShaderSourceUniform = R"(
#version 330 core
	out vec4 FragColor;

	void main(){
		FragColor = vec4(1.0,0.0,0.0,1.0);
	}
)";

const char* vertexShaderSource = R"(
#version 330 core
	layout (location = 0) in vec3 aPos;

	void main(){
		gl_Position = vec4(aPos, 1.0);
	}
)";

const char* fragmentShaderSource = R"(
#version 330 core
	out vec4 FragColor;

	void main(){
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void checkCompileErrors(unsigned int shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "B³¹d kompilacji shaderów typu: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "B³¹d linkowania programu shaderowego:\n" << infoLog << std::endl;
		}
	}
}

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "PhysixGL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "B³¹d inicjalizacji GLAD\n";
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	float vertices[] = {
		 0.0f,  0.5f, 0.0f,  // góra
		-0.5f, -0.5f, 0.0f,  // lewy dó³
		 0.5f, -0.5f, 0.0f   // prawy dó³
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Kompilacja shaderów
	unsigned int vertexUniform = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexUniform, 1, &vertexShaderSourceUniform, NULL);
	glCompileShader(vertexUniform);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	unsigned int fragmentProgramShaderUniform = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentProgramShaderUniform, 1, &fragmentShaderSourceUniform, NULL);
	glCompileShader(fragmentProgramShaderUniform);

	unsigned int shaderProgramUniform = glCreateProgram();
	glAttachShader(shaderProgramUniform, vertexUniform);
	glAttachShader(shaderProgramUniform, fragmentShader);
	glLinkProgram(shaderProgramUniform);


	int offsetLoc = glGetUniformLocation(shaderProgramUniform, "offset");
	if (offsetLoc == -1)
		std::cerr << "Nie znaleziono unfiormu";

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkCompileErrors(shaderProgram, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexUniform);
	glDeleteShader(fragmentProgramShaderUniform);

	while (!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float xOffset = sin(timeValue) * 0.5f;

		glUseProgram(shaderProgramUniform);
		glUniform3f(offsetLoc, xOffset, 0.0f, 0.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
