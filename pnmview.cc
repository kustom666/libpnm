#include <iostream>
#include <PnmImage.h>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdio>

const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec2 texcoord;"
    "out vec2 Texcoord;"
    "void main() {"
    "   Texcoord = texcoord;"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D tex;"
    "void main() {"
    "   outColor = texture(tex, Texcoord);"
    "}";

GLfloat vertices[] = {
//  Position   Color             Texcoords
    -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
     1.0f,  1.0f, 1.0f, 0.0f, // Top-right
     1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
    -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
};

int main(int argc, char const *argv[])
{
	PnmImage im;
	char fileALoad[255];
	char fileASave[255];
    int seuil = 0;

	std::cout << "Entrer le path de l'image à sobeliser" << std::endl;
	std::cin >> fileALoad;
	im.loadFromFile(fileALoad);
	im.appliqueSobel(0);
	std::cout << "Entrer le path de l'image à sauver" << std::endl;
	std::cin >> fileASave;

	sf::Window window(sf::VideoMode(im.getMWidth(), im.getMHeight()), "Preview", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

    glewExperimental = GL_TRUE;
    glewInit();

	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLuint texture;
    glGenTextures(1, &texture);
    std::cout << "Width - height : " << im.getMWidth() << im.getMHeight() << std::endl;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.getMWidth(), im.getMHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, im.getRgb());

	bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    seuil++;
                }
                else if (event.key.code == sf::Keyboard::Z)
                {
                    seuil--;
                }
            }
        }
        im.appliqueSobel(seuil);
        uint8_t* output = im.getRgb();
        glTexSubImage2D(GL_TEXTURE_2D,0,0,0,im.getMWidth(), im.getMHeight(),GL_RGB, GL_UNSIGNED_BYTE, output);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_QUADS, 0, 4);
        window.display();
        delete[] output;
    }
    im.saveToFile(fileASave);
	return 0;
}