#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer 
{
private:
	unsigned int id;
	float *data;

public:
	static void Unbind();
	static void Bind(unsigned int& id);

	VertexBuffer(const float* data);
	~VertexBuffer();

unsigned int& getId();
	const float* const getData();
};

#endif // VERTEX_BUFFER_H