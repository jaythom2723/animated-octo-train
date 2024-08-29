#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexArray
{
private:
	unsigned int id;

public:
	static void Unbind();
	static void Bind(unsigned int& id);

	VertexArray();
	~VertexArray();

	unsigned int& getId();
};

#endif // VERTEX_ARRAY_H