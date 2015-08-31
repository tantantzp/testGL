#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <cassert>
#include <vector>
#include <cstdio>

template <typename T>
class Image
{
protected:
	std::vector<T> imageData;
	unsigned w, h;
public:
	const void* getData() const { return imageData.data(); }
	void *getData() { return imageData.data(); }
	Image(){}
	Image(unsigned w, unsigned h) { resize(w, h); }

	void resize(unsigned w, unsigned h)
	{
		this->w = w;
		this->h = h;
		imageData.resize(w*h);
	}

	unsigned width() const { return w; }
	unsigned height() const { return h; }

	T &get(unsigned x, unsigned y)
	{
		if (!(x<w && y<h))
		{
			assert(false);
			return imageData[(y*w + x) % (w * h)];
		}
		return imageData[y*w + x];
	}
	T get(unsigned x, unsigned y) const
	{
		if (!(x<w && y<h))
		{
			assert(false);
			return imageData[(y*w + x) % (w * h)];
		}
		return imageData[y*w + x];
	}
	void set(unsigned x, unsigned y, T val)
	{
		assert(x < w && y < h);
		imageData[y*w + x] = val;
	}

	void saveRaw(const std::string &filename)
	{
		FILE *fp = fopen(filename.c_str(), "wb");
		fwrite(imageData.data(), sizeof(T), imageData.size(), fp);
		fclose(fp);
	}
	void readRaw(const std::string &filename)
	{
		FILE *fp = fopen(filename.c_str(), "rb");
		fread(imageData.data(), sizeof(T), imageData.size(), fp);
		fclose(fp);
	}
};

typedef Image<float> Image1f;

#endif
