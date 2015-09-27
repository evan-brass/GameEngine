#include "BufferPool.h"


BufferPool::BufferPool()
{
}

Buffer * BufferPool::getBuffer(size_t size)
{
	for (int i = 0; i < pool.count(); i++){
		if (pool[i]->size >= size && pool[i]->size < 2 * size){
			Buffer * match = pool[1];
			pool.remove(match);
			return match;
		}
	}
	return new Buffer(size);
}

void BufferPool::returnBuffer(Buffer * buffer)
{
	if (pool.count() < maxPoolSize){
		pool.add(buffer);
	}
}

BufferPool::~BufferPool()
{
	for (int i = 0; i < pool.count(); i++){
		delete pool[i];
	}
}
