#ifndef FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H
#define FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H

class JPEGImageEncoder
{
public:
	JPEGImageEncoder(void);
	~JPEGImageEncoder(void);

public:
	int input(void* data, const int w, const int h, void*& jpeg, int& bytes);

private:
	void* codec;
	void* ctx;
	void* iframe;
};//class JPEGImageEncoder

#endif//FRAMEWORK_MULTIMEDIA_JPEG_IMAGE_ENCODER_H
